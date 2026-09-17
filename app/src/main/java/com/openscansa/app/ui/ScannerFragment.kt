package com.openscansa.app.ui

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.google.android.material.snackbar.Snackbar
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.camera.BarcodeDebugDetection
import com.openscansa.app.camera.CameraScanner
import com.openscansa.app.databinding.FragmentScannerBinding
import com.openscansa.app.models.StaffProfile
import com.openscansa.app.models.LicenseData
import com.openscansa.app.viewmodel.ScannerViewModel
import com.openscansa.app.viewmodel.SettingsViewModel
import com.peachss.sadldecoder.utils.SADLUtils
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable
import java.text.SimpleDateFormat
import java.util.Locale

class ScannerFragment : Fragment() {
    private var _binding: FragmentScannerBinding? = null
    private val binding get() = requireNotNull(_binding)
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private val settingsViewModel: SettingsViewModel by activityViewModels()
    private var cameraScanner: CameraScanner? = null
    private var torchEnabled = false
    private var frozenBitmap: Bitmap? = null
    private var isFrameFrozen = false
    private var lastFrameTimeMs: Long = 0L
    private var lastDetectionTimeMs: Long = 0L
    private var userRole: String? = null
    private var isReturnMode = false
    private var requestKey = "scan_request"
    private val dateFormat = SimpleDateFormat("yyyy-MM-dd", Locale.getDefault())

    private val requestCameraPermission = registerForActivityResult(ActivityResultContracts.RequestPermission()) { granted ->
        if (granted) startCamera() else showCameraPermissionMessage()
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentScannerBinding.inflate(inflater, container, false)
        isReturnMode = arguments?.getBoolean("returnResult") ?: false
        requestKey = arguments?.getString("requestKey") ?: "scan_request"
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        fetchUserRole()

        // Set contextual scanner heading
        if (requestKey == "vehicle_driver_scan") {
            binding.tvScannerHeading.text = "Scan Driver In"
            binding.tvScannerHeading.visibility = View.VISIBLE
        } else if (requestKey == "passenger_scan") {
            binding.tvScannerHeading.text = "Scan Passenger In"
            binding.tvScannerHeading.visibility = View.VISIBLE
        } else {
            binding.tvScannerHeading.visibility = View.GONE
        }

        binding.btnHome.setOnClickListener {
            if (userRole == "admin") {
                findNavController().navigate(R.id.adminDashboardFragment)
            } else {
                findNavController().navigate(R.id.actionHubFragment)
            }
        }

        binding.btnLostQr.setOnClickListener {
            val args = Bundle().apply { 
                putBoolean("isReissueRequired", true)
                putString("requestKey", requestKey)
            }
            findNavController().navigate(R.id.action_scannerFragment_to_lostQrFragment, args)
        }

        binding.btnForgotQr.setOnClickListener {
            val args = Bundle().apply { 
                putBoolean("isReissueRequired", false)
                putString("requestKey", requestKey)
            }
            findNavController().navigate(R.id.action_scannerFragment_to_lostQrFragment, args)
        }

        // Only show recovery buttons when scanning for staff/attendance
        if (requestKey == "action_hub_scan" || requestKey == "attendance_type_scan" || requestKey == "passenger_scan" || requestKey == "vehicle_driver_scan") {
            binding.containerRecoveryButtons.visibility = View.VISIBLE
        }

        binding.torchButton.setOnClickListener {
            torchEnabled = !torchEnabled
            cameraScanner?.setTorch(torchEnabled)
            binding.torchButton.text = getString(if (torchEnabled) R.string.torch_on else R.string.torch_off)
        }

        binding.scanAgainButton.setOnClickListener {
            clearScanResult()
        }

        scannerViewModel.latestScan.observe(viewLifecycleOwner) { result ->
            val hasResult = result != null
            binding.resultCard.visibility = if (hasResult) View.VISIBLE else View.GONE
            binding.scanAgainButton.visibility = if (hasResult) View.VISIBLE else View.GONE
            result?.let {
                binding.resultValue.text = it.displayValue
                binding.resultMeta.text = buildString {
                    append(getString(R.string.scan_format, it.format))
                    if (it.licenseInfo != null || it.displayValue.contains("SA DRIVER'S LICENSE")) {
                        append(" • Licence details")
                    }
                }

                val bitmap = it.licenseImageBytes?.let { imageBytes -> decodeLicenseImageBitmap(imageBytes) }
                if (bitmap != null) {
                    binding.licenseImage.setImageBitmap(bitmap)
                    binding.licenseImage.visibility = View.VISIBLE
                } else {
                    binding.licenseImage.setImageDrawable(null)
                    binding.licenseImage.visibility = View.GONE
                }
            }
        }
        settingsViewModel.torchEnabled.observe(viewLifecycleOwner) { enabled ->
            torchEnabled = enabled
            binding.torchButton.text = getString(if (enabled) R.string.torch_on else R.string.torch_off)
            cameraScanner?.setTorch(enabled)
        }
    }

    private fun fetchUserRole() {
        val user = SupabaseManager.client.auth.currentUserOrNull() ?: return
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val email = user.email ?: return@launch
                val whitelist = SupabaseManager.client.postgrest["allowed_emails"]
                    .select {
                        filter {
                            eq("email", email)
                        }
                    }.decodeSingleOrNull<WhitelistEntry>()
                userRole = whitelist?.role ?: "guard"
            } catch (_: Exception) {
                userRole = "guard"
            }
        }
    }

    override fun onResume() { super.onResume(); ensureCameraPermission() }

    private fun ensureCameraPermission() {
        if (ContextCompat.checkSelfPermission(requireContext(), Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) startCamera()
        else requestCameraPermission.launch(Manifest.permission.CAMERA)
    }

    private fun clearScanResult() {
        isFrameFrozen = false

        frozenBitmap?.recycle()
        frozenBitmap = null

        binding.frozenFrame.setImageDrawable(null)
        binding.frozenFrame.visibility = View.GONE

        scannerViewModel.clearResult()
        binding.barcodeOverlay.clear()
        binding.scanAgainButton.visibility = View.GONE
        cameraScanner?.scanAgain()
    }

    private fun startCamera() {
        if (cameraScanner == null) cameraScanner = CameraScanner(requireContext().applicationContext)
        cameraScanner?.start(
            viewLifecycleOwner,
            binding.previewView,

            onBarcode = { value, format, _, licenseImageBytes, licenseInfo ->
                requireActivity().runOnUiThread {
                    val currentBinding = _binding
                    if (currentBinding != null && isAdded && !isFrameFrozen) {
                        
                        val normalizedValue = if (licenseInfo == null && !value.isMostlyPrintable()) {
                            value.toHexDump()
                        } else {
                            value
                        }

                        if (isReturnMode) {
                            isFrameFrozen = true
                            // Stop camera immediately to prevent further callbacks
                            cameraScanner?.stop()
                            cameraScanner = null

                            // Save to shared view model if it's a license result
                            if (licenseInfo != null) {
                                val codes = listOfNotNull(licenseInfo.licenseCode1, licenseInfo.licenseCode2, licenseInfo.licenseCode3, licenseInfo.licenseCode4)
                                    .filter { it.isNotBlank() }
                                    .joinToString(", ")
                                
                                val restrictions = listOfNotNull(licenseInfo.vehicleRestriction1, licenseInfo.vehicleRestriction2, licenseInfo.vehicleRestriction3, licenseInfo.vehicleRestriction4)
                                    .filter { it.isNotBlank() }
                                    .joinToString(", ")

                                scannerViewModel.pendingLicenseData = LicenseData(
                                    idNumber = licenseInfo.idNumber ?: "UNKNOWN",
                                    surname = licenseInfo.surname,
                                    initials = licenseInfo.initials,
                                    licenseNumber = licenseInfo.licenseNumber,
                                    issueNumber = licenseInfo.licenseIssueNo,
                                    countryOfIssue = licenseInfo.licenseCountryOfIssue,
                                    birthDate = licenseInfo.birthDate?.let { dateFormat.format(it) },
                                    validFrom = licenseInfo.licenseValidFrom?.let { dateFormat.format(it) },
                                    validTo = licenseInfo.licenseValidTo?.let { dateFormat.format(it) },
                                    gender = licenseInfo.gender,
                                    vehicleCodes = codes,
                                    vehicleRestrictions = restrictions,
                                    driverRestriction = licenseInfo.driverRestriction,
                                    pdpCode = licenseInfo.pdpCode,
                                    pdpExpiry = licenseInfo.pdpExpiry?.let { dateFormat.format(it) },
                                    photoBytes = licenseImageBytes,
                                    rawData = normalizedValue
                                )
                            }

                            val resultBundle = Bundle().apply {
                                putString("barcode", normalizedValue)
                                putBoolean("hasLicenseInfo", licenseInfo != null)
                            }
                            parentFragmentManager.setFragmentResult(requestKey, resultBundle)
                            findNavController().popBackStack()
                            return@runOnUiThread
                        }

                        isFrameFrozen = true
                        try {
                            val bitmap = currentBinding.previewView.bitmap
                            if (bitmap != null) {
                                frozenBitmap?.recycle()
                                frozenBitmap = bitmap
                                currentBinding.frozenFrame.setImageBitmap(frozenBitmap)
                                currentBinding.frozenFrame.visibility = View.VISIBLE
                            }
                        } catch (_: Exception) {}

                        scannerViewModel.onBarcodeDetected(normalizedValue, format, licenseImageBytes, licenseInfo)
                    }
                }
            },

            onFrameDetection = { detections ->
                val binding = _binding
                if (binding != null && isAdded) {
                    val now = System.currentTimeMillis()
                    // FPS calculation
                    val fps = if (lastFrameTimeMs > 0) 1000f / (now - lastFrameTimeMs) else 0f
                    lastFrameTimeMs = now

                    binding.barcodeOverlay.showBarcodes(detections.mapNotNull(BarcodeDebugDetection::bounds))

                    val detected = detections.any { it.rawValueLength > 0 }
                    if (detected) lastDetectionTimeMs = now

                    val debugText = if (now - lastDetectionTimeMs > 2000L) {
                        getString(R.string.no_barcode_detected) + "."
                    } else {
                        val first = detections.firstOrNull()
                        buildString {
                            append("FPS: ")
                            append(String.format("%.1f", fps))
                            append("\nBarcodes: ")
                            append(detections.size)
                            append("\nFormat: ")
                            append(first?.format ?: "-")
                            append("\nRaw length: ")
                            append(first?.rawValueLength ?: 0)
                            append("\nLast: ")
                            val ago = if (lastDetectionTimeMs == 0L) "-" else (now - lastDetectionTimeMs).toString() + "ms ago"
                            append(ago)
                            
                            // Add persistent native debug if available
                            if (binding.debugOverlay.text.toString().contains("Source:")) {
                                 append("\n\n")
                                 append(binding.debugOverlay.text.toString().substringAfterLast("---", ""))
                            }
                        }
                    }

                    binding.debugOverlay.text = debugText
                }
            },

            onNativeDebug = { text ->
                requireActivity().runOnUiThread {
                    val binding = _binding
                    if (binding != null && isAdded) {
                        // Update debug overlay with detailed native decoder stats
                        val currentText = binding.debugOverlay.text.toString()
                        val prefix = currentText.substringBefore("\n\n")
                        binding.debugOverlay.text = "$prefix\n\n---\n$text"
                    }
                }
            }
        )
        cameraScanner?.setTorch(torchEnabled)
    }

    private fun showCameraPermissionMessage() {
        Snackbar.make(binding.root, R.string.camera_permission_required, Snackbar.LENGTH_INDEFINITE)
            .setAction(R.string.try_again) { ensureCameraPermission() }.show()
    }

    override fun onDestroyView() {
        cameraScanner?.stop()
        cameraScanner = null

        frozenBitmap?.recycle()
        frozenBitmap = null

        _binding = null
        super.onDestroyView()
    }
    
    private fun List<BarcodeDebugDetection>.toDebugText(): String {
        if (isEmpty()) return getString(R.string.no_barcode_detected)

        return buildString {
            append("Barcodes detected in current frame: ").append(size)
            this@toDebugText.forEachIndexed { index, barcode ->
                append("\n\nBarcode ").append(index + 1)
                append("\nFormat: ").append(barcode.format)
                append("\nRaw value length: ").append(barcode.rawValueLength)
            }
        }
    }

    private fun decodeLicenseImageBitmap(imageBytes: ByteArray): Bitmap? {
        if (imageBytes.isEmpty()) return null

        // Try standard formats first (JPEG/PNG)
        val decoded = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size)
        if (decoded != null) return decoded

        // Fallback: SWIDecoder often returns raw grayscale/interleaved bytes
        return try {
            SADLUtils.getBitmapFromImageBytes(imageBytes)
        } catch (_: Exception) {
            // Last resort: square grayscale visualization
            val length = imageBytes.size.coerceAtMost(4096)
            if (length <= 0) return null
            val side = kotlin.math.ceil(kotlin.math.sqrt(length.toDouble())).toInt().coerceAtLeast(1)
            val bitmap = Bitmap.createBitmap(side, side, Bitmap.Config.ARGB_8888)
            val pixels = IntArray(side * side)
            for (i in imageBytes.indices) {
                val gray = (imageBytes[i].toInt() and 0xFF)
                val color = (0xFF shl 24) or (gray shl 16) or (gray shl 8) or gray
                pixels[i.coerceAtMost(pixels.lastIndex)] = color
            }
            bitmap.setPixels(pixels, 0, side, 0, 0, side, side)
            bitmap
        }
    }

    private fun String.isMostlyPrintable(): Boolean {
        if (isEmpty()) return false
        val printable = count { it in ' '..'~' || it == '\n' || it == '\r' || it == '\t' }
        return printable.toFloat() / length >= 0.75f
    }

    private fun String.toHexDump(): String {
        return toByteArray(Charsets.ISO_8859_1).joinToString(" ") { "%02X".format(it.toInt() and 0xFF) }
    }

    private fun saveStaffProfile(profile: StaffProfile, barcode: String) {
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                // Combine form data with barcode
                // This insert is now independent and does NOT include 'created_by'
                val data = mapOf(
                    "first_names" to profile.firstNames,
                    "last_name" to profile.lastName,
                    "id_number" to profile.idNumber,
                    "gender" to profile.gender,
                    "phone_number" to profile.phoneNumber,
                    "secondary_phone" to profile.secondaryPhone,
                    "email" to profile.email,
                    "position" to profile.position,
                    "barcode_data" to barcode
                )

                SupabaseManager.client.postgrest["profiles"].insert(data)
                
                requireActivity().runOnUiThread {
                    android.widget.Toast.makeText(requireContext(), "Staff Profile Saved Successfully!", android.widget.Toast.LENGTH_LONG).show()
                }
                
                // Clear the pending state
                scannerViewModel.pendingStaffProfile = null
                
            } catch (e: Exception) {
                requireActivity().runOnUiThread {
                    android.widget.Toast.makeText(requireContext(), "Error saving staff profile: ${e.message}", android.widget.Toast.LENGTH_LONG).show()
                }
            }
        }
    }

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
