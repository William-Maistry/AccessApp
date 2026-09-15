package com.openscansa.app.camera

import android.content.Context
import android.graphics.Rect
import android.graphics.RectF
import android.hardware.camera2.CaptureRequest
import android.os.Handler
import android.os.Looper
import android.util.Size
import android.view.Surface
import androidx.camera.camera2.interop.Camera2Interop
import androidx.camera.core.Camera
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageCapture
import androidx.camera.core.ImageCaptureException
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.view.PreviewView
import androidx.core.content.ContextCompat
import androidx.lifecycle.LifecycleOwner
import com.google.mlkit.vision.barcode.common.Barcode
import com.google.mlkit.vision.barcode.BarcodeScanner
import com.google.mlkit.vision.barcode.BarcodeScannerOptions
import com.google.mlkit.vision.barcode.BarcodeScanning
import com.google.mlkit.vision.common.InputImage
import com.google.zxing.BarcodeFormat
import com.google.zxing.BinaryBitmap
import com.google.zxing.DecodeHintType
import com.google.zxing.MultiFormatReader
import com.google.zxing.NotFoundException
import com.google.zxing.Result
import com.google.zxing.common.HybridBinarizer
import com.google.zxing.PlanarYUVLuminanceSource
import android.util.Log
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors
import com.peachss.sadldecoder.Decoder
import com.peachss.sadldecoder.utils.LicenseInfo


import androidx.camera.camera2.interop.ExperimentalCamera2Interop

internal fun decodePayloadText(bytes: ByteArray): String {
    val candidates = listOf(
        decodeText(bytes, Charsets.UTF_8),
        decodeText(bytes, Charsets.ISO_8859_1),
        decodeText(bytes, try { java.nio.charset.Charset.forName("Cp1252") } catch (_: Exception) { null })
    ).filterNotNull()

    candidates.firstOrNull { isMostlyPrintable(it) }?.let { return it }

    val isoText = decodeText(bytes, Charsets.ISO_8859_1)
    if (!isoText.isNullOrEmpty()) return isoText.trim()

    val utf8Text = decodeText(bytes, Charsets.UTF_8)
    if (!utf8Text.isNullOrEmpty()) return utf8Text.trim()

    return String(bytes, Charsets.ISO_8859_1).trim()
}

private fun decodeText(bytes: ByteArray, charset: java.nio.charset.Charset?): String? {
    if (charset == null) return null
    return try {
        String(bytes, charset)
    } catch (_: Exception) {
        null
    }
}

private fun isMostlyPrintable(text: String): Boolean {
    if (text.isEmpty()) return false
    val printable = text.count { it in ' '..'~' || it == '\n' || it == '\r' || it == '\t' }
    return printable.toFloat() / text.length >= 0.6f
}

data class Quad<out A, out B, out C, out D>(
    val first: A,
    val second: B,
    val third: C,
    val fourth: D
)

class CameraScanner(private val context: Context) {
    private var executor: ExecutorService? = null
    private var imageCapture: ImageCapture? = null
    private var scanner: BarcodeScanner? = null
    private val mainHandler = Handler(Looper.getMainLooper())
    private var cameraProvider: ProcessCameraProvider? = null
    private var camera: Camera? = null
    private val zxingReader = MultiFormatReader()
    private val pdf417Hints = mapOf(
        DecodeHintType.POSSIBLE_FORMATS to listOf(BarcodeFormat.PDF_417),
        DecodeHintType.TRY_HARDER to true,
        DecodeHintType.ALSO_INVERTED to true,
        DecodeHintType.PURE_BARCODE to false
    )
    private val qrHints = mapOf(
        DecodeHintType.POSSIBLE_FORMATS to listOf(BarcodeFormat.QR_CODE),
        DecodeHintType.TRY_HARDER to true,
        DecodeHintType.ALSO_INVERTED to true,
        DecodeHintType.PURE_BARCODE to false
    )
    private val code128Hints = mapOf(
        DecodeHintType.POSSIBLE_FORMATS to listOf(BarcodeFormat.CODE_128),
        DecodeHintType.TRY_HARDER to true,
        DecodeHintType.ALSO_INVERTED to true,
        DecodeHintType.PURE_BARCODE to false
    )
    private val zxingHints = mapOf(
        DecodeHintType.POSSIBLE_FORMATS to listOf(
            BarcodeFormat.PDF_417,
            BarcodeFormat.QR_CODE,
            BarcodeFormat.CODE_128
        ),
        DecodeHintType.TRY_HARDER to true,
        DecodeHintType.ALSO_INVERTED to true,
        DecodeHintType.PURE_BARCODE to false
    )
    @Volatile private var scanningPaused = false
    @Volatile private var processingFrame = false

    fun start(
        owner: LifecycleOwner,
        previewView: PreviewView,
        onBarcode: (String, String, RectF?, ByteArray?, LicenseInfo?) -> Unit,
        onFrameDetection: (List<BarcodeDebugDetection>) -> Unit,
        onNativeDebug: (String) -> Unit
    ) {
        Decoder.initialize()
        val providerFuture = ProcessCameraProvider.getInstance(context)
        providerFuture.addListener({
            val provider = providerFuture.get()
            cameraProvider = provider
            executor?.shutdownNow()
            executor = Executors.newSingleThreadExecutor()
            scanner = BarcodeScanning.getClient(
                BarcodeScannerOptions.Builder()
                    .setBarcodeFormats(
                        Barcode.FORMAT_PDF417,
                        Barcode.FORMAT_QR_CODE,
                        Barcode.FORMAT_CODE_128
                    )
                    .build()
            )

            val targetRotation = previewView.display?.rotation ?: Surface.ROTATION_0
            val preview = Preview.Builder()
                .setTargetRotation(targetRotation)
                .build()
                .also { it.surfaceProvider = previewView.surfaceProvider }

            val capture = ImageCapture.Builder()
                .setCaptureMode(ImageCapture.CAPTURE_MODE_MAXIMIZE_QUALITY)
                .setTargetRotation(targetRotation)
                .build()

            imageCapture = capture

            val analysis = buildAnalysis(targetRotation)
            analysis.setAnalyzer(executor!!) { imageProxy ->
                if (scanningPaused || processingFrame || executor == null) {
                    imageProxy.close()
                    return@setAnalyzer
                }

                val mediaImage = imageProxy.image
                if (mediaImage == null) {
                    imageProxy.close()
                    return@setAnalyzer
                }

                processingFrame = true
                val frameStart = System.currentTimeMillis()

                // Try ZXing decode first with a full-frame PDF417 preflight before region detection.
                val width = imageProxy.width
                val height = imageProxy.height
                val rotation = imageProxy.imageInfo.rotationDegrees
                val zxingAttempted = true

                val yData = extractYPlane(imageProxy)
                val (zxingData, zxingWidth, zxingHeight) = rotateYPlaneIfNeeded(yData, width, height, rotation)

                var zxingResult: Result? = null
                var zxingError: String? = null
                var zxingTime = 0L
                try {
                    val startZ = System.currentTimeMillis()

                    val (nativeText, nativeLicenseImageBytes, nativeLicenseInfo) = tryDecodeWithCrops(
                        zxingData,
                        zxingWidth,
                        zxingHeight
                    )

                    mainHandler.post {
                        if (scanningPaused) return@post
                        onNativeDebug(
                            """
                    Native Decoder
                    --------------
                    Resolution : ${zxingWidth} x ${zxingHeight}
                    Rotation   : $rotation
                    Result     : ${
                        if (nativeLicenseInfo == null && nativeText.isNullOrBlank())
                            "NO PDF417 FOUND"
                        else
                            "SUCCESS"
                    }
                    Length     : ${nativeText?.length ?: 0}
                            """.trimIndent()
                        )
                    }

                    if (nativeLicenseInfo != null || !nativeText.isNullOrBlank()) {
                        Log.d(TAG, "Native PDF417 decoder succeeded")

                        scanningPaused = true

                        mainHandler.post {
                            onBarcode(
                                nativeText ?: "",
                                "PDF417",
                                null,
                                nativeLicenseImageBytes,
                                nativeLicenseInfo
                            )
                        }

                        processingFrame = false
                        imageProxy.close()
                        return@setAnalyzer
                    }

                    zxingResult = tryDecodeWithZXing(zxingData, zxingWidth, zxingHeight)
                    if (zxingResult == null && zxingWidth != zxingHeight) {
                        val (rot90, rot90Width, rot90Height) = rotateYPlaneIfNeeded(zxingData, zxingWidth, zxingHeight, 90)
                        zxingResult = tryDecodeWithZXing(rot90, rot90Width, rot90Height)
                    }
                    if (zxingResult == null && zxingWidth != zxingHeight) {
                        val (rot270, rot270Width, rot270Height) = rotateYPlaneIfNeeded(zxingData, zxingWidth, zxingHeight, 270)
                        zxingResult = tryDecodeWithZXing(rot270, rot270Width, rot270Height)
                    }

                    zxingTime = System.currentTimeMillis() - startZ
                } catch (e: Exception) {
                    zxingError = e.message ?: e.javaClass.simpleName
                    zxingTime = System.currentTimeMillis() - frameStart
                }

                if (zxingResult != null) {
                    val (barcodeValue, licenseImageBytes, licenseInfo, debugStr) = if (zxingResult.barcodeFormat == BarcodeFormat.PDF_417 && zxingResult.rawBytes != null) {
                        handleDetectedBarcode("ZXing", zxingResult.rawBytes, "PDF417")
                    } else {
                        Quad(zxingResult.text, null, null, "Source: ZXing (Non-PDF417)")
                    }

                    val zxingFormatName = when (zxingResult.barcodeFormat) {
                        BarcodeFormat.PDF_417 -> "PDF417"
                        BarcodeFormat.QR_CODE -> "QR Code"
                        BarcodeFormat.CODE_128 -> "Code 128"
                        else -> zxingResult.barcodeFormat.name
                    }
                    
                    mainHandler.post { onNativeDebug(debugStr) }
                    
                    val detection = BarcodeDebugDetection(
                        bounds = null,
                        format = zxingFormatName,
                        rawValueLength = barcodeValue?.length ?: 0,
                        displayValueLength = barcodeValue?.length ?: 0,
                        isRawNull = barcodeValue == null,
                        processingTimeMs = zxingTime,
                        timestampMs = frameStart
                    )
                    mainHandler.post { onFrameDetection(listOf(detection)) }
                    
                    mainHandler.post { onBarcode(barcodeValue ?: "", zxingFormatName, null, licenseImageBytes, licenseInfo) }
                    
                    processingFrame = false
                    imageProxy.close()
                    return@setAnalyzer
                } else {
                    Log.d(TAG, "Frame resolved - ZXingResult=none error=$zxingError decodeMs=$zxingTime")
                }

                // If ZXing didn't find anything, fall back to ML Kit as before
                val currentScanner = scanner
                if (scanningPaused || currentScanner == null) {
                    imageProxy.close()
                    processingFrame = false
                    return@setAnalyzer
                }

                val image = InputImage.fromMediaImage(mediaImage, rotation)
                currentScanner.process(image)
                    ?.addOnSuccessListener { codes ->
                        val frameProcessingTime = System.currentTimeMillis() - frameStart
                        val debugDetections = codes.map { code ->
                            BarcodeDebugDetection(
                                bounds = code.boundingBox?.let { boundingBox ->
                                    mapPreviewBounds(
                                        boundingBox,
                                        width,
                                        height,
                                        rotation,
                                        previewView.width,
                                        previewView.height
                                    )
                                },
                                format = formatName(code.format),
                                rawValueLength = code.rawValue?.length ?: 0,
                                displayValueLength = code.displayValue?.length ?: 0,
                                isRawNull = code.rawValue == null,
                                processingTimeMs = frameProcessingTime,
                                timestampMs = frameStart
                            )
                        }

                        // Log per-frame details
                        try {
                            val anyDetected = codes.isNotEmpty()
                            val formats = codes.joinToString { formatName(it.format) }
                            val boxes = codes.mapNotNull { code ->
                                code.boundingBox?.let { "(${it.left},${it.top},${it.right},${it.bottom})" }
                            }
                            if (anyDetected) {
                                Log.d(TAG, "Frame resolved - resolution=${width}x${height} rotation=${rotation} ZXingAttempted=$zxingAttempted MLKitAttempted=true MLKitResult=success count=${codes.size} formats=$formats boxes=${boxes.joinToString()} decodeMs=${frameProcessingTime}")
                            } else {
                                Log.d(TAG, "Frame resolved - resolution=${width}x${height} rotation=${rotation} ZXingAttempted=$zxingAttempted MLKitAttempted=true MLKitResult=none count=0 no barcode detected decodeMs=${frameProcessingTime}")
                            }
                            codes.forEachIndexed { idx, code ->
                                Log.d(TAG, "MLKit Barcode[$idx]: format=${formatName(code.format)} rawLen=${code.rawValue?.length ?: 0} displayLen=${code.displayValue?.length ?: 0} rawNull=${code.rawValue == null} box=${code.boundingBox}")
                            }
                        } catch (_: Exception) {}

                        mainHandler.post {
                            onFrameDetection(debugDetections)
                        }
                        val matchedCode = codes.firstOrNull { !it.rawValue.isNullOrBlank() || !it.displayValue.isNullOrBlank() }
                        if (matchedCode != null) {
                            val value = matchedCode.rawValue?.takeIf { it.isNotBlank() }
                                ?: matchedCode.displayValue.orEmpty()
                            val format = formatName(matchedCode.format)

                            val (barcodeValue, licenseImageBytes, licenseInfo, debugStr) = if (format == "PDF417") {
                                // PDF417 from ML Kit often only provides String. We must convert to binary via ISO-8859-1 mapping.
                                handleDetectedBarcode("MLKit", value.toByteArray(Charsets.ISO_8859_1), format)
                            } else {
                                Quad(value, null, null, "Source: MLKit (Non-PDF417)")
                            }

                            mainHandler.post { onNativeDebug(debugStr) }

                            mainHandler.post {
                                onBarcode(
                                    barcodeValue ?: "",
                                    format,
                                    debugDetections[codes.indexOf(matchedCode)].bounds,
                                    licenseImageBytes,
                                    licenseInfo
                                )
                            }
                        } else {
                            val fallbackCode = codes.firstOrNull { it.boundingBox != null }
                            fallbackCode?.boundingBox?.let { box ->
                                val regionResult = tryDecodeZXingRegion(zxingData, zxingWidth, zxingHeight, box)
                                if (regionResult != null) {
                                    Log.d(TAG, "Fallback ZXing region decode succeeded: ${formatName(regionResult.barcodeFormat)} length=${regionResult.text.length}")
                                    mainHandler.post {
                                        onBarcode(
                                            regionResult.text,
                                            formatName(regionResult.barcodeFormat),
                                            mapPreviewBounds(box, zxingWidth, zxingHeight, 0, previewView.width, previewView.height),
                                            null,
                                            null
                                        )
                                    }
                                } else {
                                    Log.d(TAG, "Fallback ZXing region decode failed for bounding box $box")
                                }
                            }
                        }
                    }
                    ?.addOnFailureListener {
                        // Skip frame without surfacing a scanner error to keep the stream continuous.
                    }
                    ?.addOnCompleteListener {
                        processingFrame = false
                        imageProxy.close()
                    }
            }

            provider.unbindAll()
            camera = provider.bindToLifecycle(
                owner,
                CameraSelector.DEFAULT_BACK_CAMERA,
                preview,
                analysis,
                capture
            )
        }, ContextCompat.getMainExecutor(context))
    }

    fun captureHighQualityImage(
        onCaptured: (android.net.Uri) -> Unit,
        onError: (Exception) -> Unit
    ) {
        val capture = imageCapture ?: run {
            onError(IllegalStateException("ImageCapture is not initialized"))
            return
        }

        val file = java.io.File.createTempFile(
            "openscansa_capture_",
            ".jpg",
            context.cacheDir
        )

        val outputOptions = ImageCapture.OutputFileOptions.Builder(file)
            .build()

        capture.takePicture(
            outputOptions,
            ContextCompat.getMainExecutor(context),
            object : ImageCapture.OnImageSavedCallback {

                override fun onImageSaved(
                    outputFileResults: ImageCapture.OutputFileResults
                ) {
                    onCaptured(android.net.Uri.fromFile(file))
                }

                override fun onError(exception: ImageCaptureException) {
                    onError(exception)
                }
            }
        )
    }

    fun scanAgain() { scanningPaused = false }

    fun setTorch(enabled: Boolean) { camera?.cameraControl?.enableTorch(enabled) }

    fun stop() {
        mainHandler.removeCallbacksAndMessages(null)
        scanningPaused = true
        cameraProvider?.unbindAll()
        camera = null
        scanner?.close()
        scanner = null
        executor?.shutdownNow()
        executor = null
    }

    private fun buildAnalysis(targetRotation: Int): ImageAnalysis {
        val candidates = listOf(
            Size(1080, 1920),
            Size(720, 1280),
            Size(720, 960),
            Size(480, 640)
        )

        for (candidate in candidates) {
            val builder = ImageAnalysis.Builder()
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .setTargetResolution(candidate)
                .setTargetRotation(targetRotation)

            configureContinuousCaptureRequestOptions(builder)

            try {
                return builder.build()
            } catch (_: IllegalArgumentException) {
                // Fall back to the next supported size.
            }
        }

        val fallbackBuilder = ImageAnalysis.Builder()
            .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
            .setTargetRotation(targetRotation)
            .setTargetResolution(Size(480, 640))
        configureContinuousCaptureRequestOptions(fallbackBuilder)
        return fallbackBuilder.build()
    }

    @OptIn(ExperimentalCamera2Interop::class)
    private fun configureContinuousCaptureRequestOptions(builder: ImageAnalysis.Builder) {
        val extender = Camera2Interop.Extender(builder)
        extender.setCaptureRequestOption(
            CaptureRequest.CONTROL_AF_MODE,
            CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE
        )
        extender.setCaptureRequestOption(
            CaptureRequest.CONTROL_AE_MODE,
            CaptureRequest.CONTROL_AE_MODE_ON
        )
        extender.setCaptureRequestOption(
            CaptureRequest.CONTROL_AWB_MODE,
            CaptureRequest.CONTROL_AWB_MODE_AUTO
        )
    }

    private fun formatName(format: Int): String = when (format) {
        Barcode.FORMAT_PDF417 -> "PDF417"
        Barcode.FORMAT_QR_CODE -> "QR Code"
        Barcode.FORMAT_CODE_128 -> "Code 128"
        Barcode.FORMAT_AZTEC -> "Aztec"
        Barcode.FORMAT_CODABAR -> "Codabar"
        Barcode.FORMAT_CODE_39 -> "Code 39"
        Barcode.FORMAT_CODE_93 -> "Code 93"
        Barcode.FORMAT_DATA_MATRIX -> "Data Matrix"
        Barcode.FORMAT_EAN_8 -> "EAN-8"
        Barcode.FORMAT_EAN_13 -> "EAN-13"
        Barcode.FORMAT_ITF -> "ITF"
        Barcode.FORMAT_UPC_A -> "UPC-A"
        Barcode.FORMAT_UPC_E -> "UPC-E"
        else -> "Unknown"
    }

    private fun formatName(format: BarcodeFormat): String = when (format) {
        BarcodeFormat.PDF_417 -> "PDF417"
        BarcodeFormat.QR_CODE -> "QR Code"
        BarcodeFormat.CODE_128 -> "Code 128"
        BarcodeFormat.AZTEC -> "Aztec"
        BarcodeFormat.CODABAR -> "Codabar"
        BarcodeFormat.CODE_39 -> "Code 39"
        BarcodeFormat.CODE_93 -> "Code 93"
        BarcodeFormat.DATA_MATRIX -> "Data Matrix"
        BarcodeFormat.EAN_8 -> "EAN-8"
        BarcodeFormat.EAN_13 -> "EAN-13"
        BarcodeFormat.ITF -> "ITF"
        BarcodeFormat.UPC_A -> "UPC-A"
        BarcodeFormat.UPC_E -> "UPC-E"
        else -> format.name
    }

    private fun tryDecodeEntireFrame(
        yData: ByteArray,
        width: Int,
        height: Int
    ): String? {

        return NativeScanner.decodePDF417(
            yData,
            width,
            height
        )

    }

    private fun handleDetectedBarcode(source: String, rawBytes: ByteArray, format: String): Quad<String?, ByteArray?, LicenseInfo?, String> {
        val (normalizedBytes, replacementCount) = normalizeLicenseBytesWithCount(rawBytes)
        val saDlPayload = findSaDlPayload(normalizedBytes)
        
        val baseDebug = "Source: $source\nNormalizations: $replacementCount"
        
        if (saDlPayload != null) {
            try {
                val licenseInfo = Decoder.decodeLicenseBarcode(saDlPayload)
                if (licenseInfo != null) {
                    var photoBytes: ByteArray? = null
                    licenseInfo.rawImage?.let { rawImage ->
                        if (rawImage.isNotEmpty()) {
                            try {
                                photoBytes = Decoder.getDecodedPhoto(Decoder.toPrimitives(rawImage))
                            } catch (e: Exception) {
                                Log.w(TAG, "Photo extraction failed: $e")
                            }
                        }
                    }
                    return Quad("SA Driver's License Decoded", photoBytes, licenseInfo, "$baseDebug\nSignature: FOUND\nStatus: DECODED")
                }
            } catch (e: Exception) {
                Log.w(TAG, "SA DL decoding failed: $e")
            }
            return Quad("ENCRYPTED_SA_DL_DETECTED", null, null, "$baseDebug\nSignature: FOUND\nStatus: FAILED")
        }

        val text = decodePayloadText(normalizedBytes)
        val image = extractLicenseImageBytes(normalizedBytes)
        return Quad(text, image, null, "$baseDebug\nSignature: NOT FOUND")
    }

    private fun normalizeLicenseBytesWithCount(src: ByteArray): Pair<ByteArray, Int> {
        val out = java.io.ByteArrayOutputStream()
        var replacements = 0
        var i = 0
        while (i < src.size) {
            val b = src[i].toInt() and 0xFF
            if (b == 0x3F && i + 1 < src.size && (src[i + 1].toInt() and 0xFF) == 0x3F) {
                out.write(0x00)
                i += 2
                replacements++
            } else {
                out.write(src[i].toInt())
                i++
            }
        }
        return Pair(out.toByteArray(), replacements)
    }

    private fun tryDecodeWithCrops(
        yData: ByteArray,
        width: Int,
        height: Int
    ): Triple<String?, ByteArray?, LicenseInfo?> {

        Log.d(TAG, "Starting Native PDF417 Scan")

        val resultBytes = NativeScanner.decodePDF417Bytes(yData, width, height)

        if (resultBytes == null || resultBytes.isEmpty()) {
            return Triple(null, null, null)
        }

        val (text, image, info, _) = handleDetectedBarcode("Native", resultBytes, "PDF417")
        return Triple(text, image, info)
    }

    private fun processPdf417Bytes(rawBytes: ByteArray): Triple<String?, ByteArray?, LicenseInfo?> {
        val (text, image, info, _) = handleDetectedBarcode("Scanner", rawBytes, "PDF417")
        return Triple(text, image, info)
    }

    private fun findSaDlPayload(bytes: ByteArray): ByteArray? {
        if (bytes.size < 720) return null
        
        // Pattern: 0x01, 0x9B (The standard start of a South African Driver's License header)
        for (i in 0..bytes.size - 720) {
            val b0 = bytes[i].toInt() and 0xFF
            val b1 = bytes[i + 1].toInt() and 0xFF
            
            if (b0 == 0x01 && b1 == 0x9B) {
                // Signature matched. Look for version byte (0x45) or standard markers (0x03, 0x09)
                val window = bytes.sliceArray(i until i + 10)
                val hasMarker = window.any { (it.toInt() and 0xFF) == 0x45 } || 
                                (window[2].toInt() and 0xFF) in listOf(0x03, 0x08, 0x09)
                
                if (hasMarker) {
                    Log.d(TAG, "Found SA DL header at offset $i: ${bytes.sliceArray(i until i + 16).toHexString()}")
                    return bytes.sliceArray(i until i + 720)
                }
            }
        }
        return null
    }

    private fun normalizeLicenseBytes(src: ByteArray): ByteArray {
        return normalizeLicenseBytesWithCount(src).first
    }

    private fun extractLicenseImageBytes(bytes: ByteArray): ByteArray? {
        if (bytes.isEmpty()) return null

        // Normalize the bytes first
        val normalized = normalizeLicenseBytes(bytes)
        
        // Search for JPEG, PNG, or GIF headers
        val pngHeader = byteArrayOf(0x89.toByte(), 0x50.toByte(), 0x4E.toByte(), 0x47.toByte(), 0x0D.toByte(), 0x0A.toByte(), 0x1A.toByte(), 0x0A.toByte())
        val jpegHeader = byteArrayOf(0xFF.toByte(), 0xD8.toByte(), 0xFF.toByte())
        val gifHeader = byteArrayOf(0x47.toByte(), 0x49.toByte(), 0x46.toByte(), 0x38.toByte())

        // Search through normalized bytes for image headers
        for (i in 0 until maxOf(0, normalized.size - 7)) {
            // Check for PNG header
            if (i + 8 <= normalized.size && normalized.copyOfRange(i, i + 8).contentEquals(pngHeader)) {
                Log.d(TAG, "Found PNG header at offset $i")
                return normalized.copyOfRange(i, normalized.size)
            }
            // Check for JPEG header
            if (i + 3 <= normalized.size && normalized.copyOfRange(i, i + 3).contentEquals(jpegHeader)) {
                Log.d(TAG, "Found JPEG header at offset $i")
                return normalized.copyOfRange(i, normalized.size)
            }
            // Check for GIF header
            if (i + 4 <= normalized.size && normalized.copyOfRange(i, i + 4).contentEquals(gifHeader)) {
                Log.d(TAG, "Found GIF header at offset $i")
                return normalized.copyOfRange(i, normalized.size)
            }
        }

        return null
    }

    // (kept single ByteArray.toHexString implementation below)

    private fun decodePayload(bytes: ByteArray): String = decodePayloadText(bytes)

    private fun ByteArray.toHexString(): String = joinToString(separator = " ") {
        "%02X".format(it.toInt() and 0xFF)
    }



    private fun tryDecodeWithHints(
        reader: MultiFormatReader,
        yData: ByteArray,
        width: Int,
        height: Int,
        hints: Map<DecodeHintType, Any>
    ): Result? {
        return try {
            val source = PlanarYUVLuminanceSource(yData, width, height, 0, 0, width, height, false)
            val bitmap = BinaryBitmap(HybridBinarizer(source))
            reader.reset()
            reader.decode(bitmap, hints)
        } catch (_: NotFoundException) {
            null
        } catch (_: Exception) {
            null
        }
    }

    private fun tryDecodeWithZXing(
        yData: ByteArray,
        width: Int,
        height: Int
    ): Result? {
        return tryDecodeWithHints(
            zxingReader,
            yData,
            width,
            height,
            zxingHints
        )
    }

    private fun tryDecodeZXingRegion(
        yData: ByteArray,
        width: Int,
        height: Int,
        region: Rect
    ): Result? {
        if (region.width() <= 0 || region.height() <= 0) return null
        val cropped = cropYRegion(yData, width, height, region)
        return tryDecodeWithHints(
            zxingReader,
            cropped,
            region.width(),
            region.height(),
            zxingHints
        )
    }

    private companion object {
        const val SCAN_PAUSE_MS = 2_000L
        const val TAG = "OpenScanSA"
    }
}

data class BarcodeDebugDetection(
    val bounds: RectF?,
    val format: String,
    val rawValueLength: Int,
    val displayValueLength: Int = 0,
    val isRawNull: Boolean = false,
    val processingTimeMs: Long = 0,
    val timestampMs: Long = 0
)

private fun extractYPlane(imageProxy: androidx.camera.core.ImageProxy): ByteArray {
    val image = imageProxy.image ?: return ByteArray(0)
    val yPlane = image.planes[0]
    val width = imageProxy.width
    val height = imageProxy.height
    val rowStride = yPlane.rowStride
    val buffer = yPlane.buffer.duplicate()
    val yData = ByteArray(width * height)

    if (rowStride == width) {
        buffer.get(yData)
        return yData
    }

    for (row in 0 until height) {
        buffer.position(row * rowStride)
        buffer.get(yData, row * width, width)
    }
    return yData
}

private fun rotateYPlaneIfNeeded(yData: ByteArray, width: Int, height: Int, rotation: Int): Triple<ByteArray, Int, Int> {
    return when (rotation) {
        90 -> {
            val rotated = ByteArray(yData.size)
            for (row in 0 until height) {
                for (col in 0 until width) {
                    rotated[col * height + (height - row - 1)] = yData[row * width + col]
                }
            }
            Triple(rotated, height, width)
        }
        180 -> {
            val rotated = ByteArray(yData.size)
            for (i in yData.indices) {
                rotated[yData.size - 1 - i] = yData[i]
            }
            Triple(rotated, width, height)
        }
        270 -> {
            val rotated = ByteArray(yData.size)
            for (row in 0 until height) {
                for (col in 0 until width) {
                    rotated[(width - col - 1) * height + row] = yData[row * width + col]
                }
            }
            Triple(rotated, height, width)
        }
        else -> Triple(yData, width, height)
    }
}

private fun mapPreviewBounds(
    boundingBox: Rect,
    imageWidth: Int,
    imageHeight: Int,
    rotation: Int,
    previewWidth: Int,
    previewHeight: Int
): RectF? {
    if (previewWidth <= 0 || previewHeight <= 0) return null

    val effectiveWidth = if (rotation == 90 || rotation == 270) imageHeight else imageWidth
    val effectiveHeight = if (rotation == 90 || rotation == 270) imageWidth else imageHeight

    val scale = maxOf(
        previewWidth / effectiveWidth.toFloat(),
        previewHeight / effectiveHeight.toFloat()
    )
    val scaledWidth = effectiveWidth * scale
    val scaledHeight = effectiveHeight * scale
    val dx = (previewWidth - scaledWidth) / 2f
    val dy = (previewHeight - scaledHeight) / 2f

    return RectF(
        boundingBox.left * scale + dx,
        boundingBox.top * scale + dy,
        boundingBox.right * scale + dx,
        boundingBox.bottom * scale + dy
    ).apply {
        left = left.coerceIn(0f, previewWidth.toFloat())
        top = top.coerceIn(0f, previewHeight.toFloat())
        right = right.coerceIn(0f, previewWidth.toFloat())
        bottom = bottom.coerceIn(0f, previewHeight.toFloat())
    }
}

private fun detectBarcodeRegions(yData: ByteArray, width: Int, height: Int): List<Rect> {
    val regions = mutableListOf<Rect>()
    val blockSize = 32
    for (row in 0 until height step blockSize) {
        for (col in 0 until width step blockSize) {
            var sum = 0
            for (y in 0 until blockSize) {
                val r = row + y
                if (r >= height) break
                val offset = r * width + col
                for (x in 0 until blockSize) {
                    val c = col + x
                    if (c >= width) break
                    sum += yData[offset + x].toInt() and 0xFF
                }
            }
            val mean = sum / (blockSize * blockSize)
            if (mean in 30..220) {
                val left = col
                val top = row
                val right = minOf(width, col + blockSize)
                val bottom = minOf(height, row + blockSize)
                regions.add(Rect(left, top, right, bottom))
            }
        }
    }
    return regions.ifEmpty { listOf(Rect(0, 0, width, height)) }
}

private fun cropYRegion(yData: ByteArray, width: Int, height: Int, region: Rect): ByteArray {
    val cropped = ByteArray(region.width() * region.height())
    for (row in 0 until region.height()) {
        val srcPos = (region.top + row) * width + region.left
        val dstPos = row * region.width()
        System.arraycopy(yData, srcPos, cropped, dstPos, region.width())
    }
    return cropped
}

private fun enhanceContrast(yData: ByteArray): ByteArray {
    var min = 255
    var max = 0
    for (byte in yData) {
        val v = byte.toInt() and 0xFF
        min = minOf(min, v)
        max = maxOf(max, v)
    }
    val contrast = max - min
    if (contrast <= 0) return yData
    val result = ByteArray(yData.size)
    for (i in yData.indices) {
        val normalized = ((yData[i].toInt() and 0xFF) - min) * 255 / contrast
        result[i] = normalized.coerceIn(0, 255).toByte()
    }
    return result
}

private fun upscaleYPlane(yData: ByteArray, width: Int, height: Int, scale: Int): ByteArray {
    val scaledWidth = width * scale
    val scaledHeight = height * scale
    val scaled = ByteArray(scaledWidth * scaledHeight)
    for (row in 0 until scaledHeight) {
        val srcRow = row / scale
        for (col in 0 until scaledWidth) {
            scaled[row * scaledWidth + col] = yData[srcRow * width + (col / scale)]
        }
    }
    return scaled
}
