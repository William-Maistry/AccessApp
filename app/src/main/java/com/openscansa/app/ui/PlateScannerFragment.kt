package com.openscansa.app.ui

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.google.mlkit.vision.common.InputImage
import com.google.mlkit.vision.text.TextRecognition
import com.google.mlkit.vision.text.latin.TextRecognizerOptions
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentPlateScannerBinding
import com.openscansa.app.models.VehicleData
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

@androidx.annotation.OptIn(androidx.camera.core.ExperimentalGetImage::class)
class PlateScannerFragment : Fragment() {
    private var _binding: FragmentPlateScannerBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    
    private var cameraProvider: ProcessCameraProvider? = null
    private var cameraExecutor: ExecutorService? = null
    private val textRecognizer = TextRecognition.getClient(TextRecognizerOptions.DEFAULT_OPTIONS)
    
    @Volatile private var isScanningPaused = false
    private var lastScannedPlate: String = ""

    private val requestCameraPermission = registerForActivityResult(ActivityResultContracts.RequestPermission()) { granted ->
        if (granted) startCamera() else showCameraPermissionMessage()
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentPlateScannerBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        cameraExecutor = Executors.newSingleThreadExecutor()

        binding.btnBack.setOnClickListener {
            findNavController().popBackStack()
        }

        binding.btnScanAgain.setOnClickListener {
            binding.resultCard.visibility = View.GONE
            isScanningPaused = false
        }

        binding.btnProceed.setOnClickListener {
            if (lastScannedPlate.isNotEmpty()) {
                val args = Bundle().apply {
                    putString("plateNumber", lastScannedPlate)
                }
                findNavController().navigate(R.id.action_plateScannerFragment_to_vehicleCategoryFragment, args)
            }
        }

        ensureCameraPermission()
    }

    private fun ensureCameraPermission() {
        if (ContextCompat.checkSelfPermission(requireContext(), Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) {
            startCamera()
        } else {
            requestCameraPermission.launch(Manifest.permission.CAMERA)
        }
    }

    private fun startCamera() {
        val providerFuture = ProcessCameraProvider.getInstance(requireContext())
        providerFuture.addListener({
            try {
                cameraProvider = providerFuture.get()
                
                val preview = Preview.Builder().build().also {
                    it.surfaceProvider = binding.previewView.surfaceProvider
                }

                val imageAnalysis = ImageAnalysis.Builder()
                    .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                    .build()

                imageAnalysis.setAnalyzer(cameraExecutor!!) { imageProxy ->
                    val mediaImage = imageProxy.image
                    if (mediaImage == null || isScanningPaused) {
                        imageProxy.close()
                        return@setAnalyzer
                    }

                    val image = InputImage.fromMediaImage(mediaImage, imageProxy.imageInfo.rotationDegrees)
                    textRecognizer.process(image)
                        .addOnSuccessListener { visionText ->
                            processRecognizedText(visionText.text)
                        }
                        .addOnCompleteListener {
                            imageProxy.close()
                        }
                }

                cameraProvider?.unbindAll()
                cameraProvider?.bindToLifecycle(
                    viewLifecycleOwner,
                    CameraSelector.DEFAULT_BACK_CAMERA,
                    preview,
                    imageAnalysis
                )
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Failed to open camera: ${e.message}", Toast.LENGTH_SHORT).show()
            }
        }, ContextCompat.getMainExecutor(requireContext()))
    }

    private fun processRecognizedText(rawText: String) {
        if (isScanningPaused || rawText.isBlank()) return

        // Extract possible number plate text from lines
        val lines = rawText.split("\n")
        for (line in lines) {
            val sanitized = line.replace(" ", "").replace("-", "").trim().uppercase()
            // South African plates typically have 6 to 9 alphanumeric chars
            if (sanitized.length in 4..12 && sanitized.any { it.isDigit() } && sanitized.any { it.isLetter() }) {
                lookupPlate(sanitized)
                break
            }
        }
    }

    private fun lookupPlate(plateNumber: String) {
        isScanningPaused = true
        lastScannedPlate = plateNumber
        
        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                val match = SupabaseManager.client.postgrest["vehicle_registrations"]
                    .select {
                        filter {
                            eq("licence_number", plateNumber)
                        }
                    }.decodeSingleOrNull<VehicleData>()

                if (match != null) {
                    scannerViewModel.pendingVehicleData = match
                    displayVehicleDetails(match, plateNumber)
                } else {
                    scannerViewModel.pendingVehicleData = VehicleData(licenceNumber = plateNumber)
                    val fallback = VehicleData(make = "Unregistered Vehicle", model = "")
                    displayVehicleDetails(fallback, plateNumber)
                }
            } catch (e: Exception) {
                scannerViewModel.pendingVehicleData = VehicleData(licenceNumber = plateNumber)
                val fallback = VehicleData(make = "Unregistered Vehicle", model = "")
                displayVehicleDetails(fallback, plateNumber)
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun displayVehicleDetails(vehicle: VehicleData, plateNumber: String) {
        binding.tvPlateNumber.text = plateNumber
        binding.tvVehicleMakeModel.text = "${vehicle.make ?: "Unknown Make"} ${vehicle.model ?: ""}".trim()
        binding.tvVehicleColour.text = if (vehicle.colour != null) "Colour: ${vehicle.colour}" else ""
        binding.tvVehicleExpiry.text = if (vehicle.expiryDate != null) "Disc Expiry: ${vehicle.expiryDate}" else ""
        
        binding.resultCard.visibility = View.VISIBLE
    }

    private fun showCameraPermissionMessage() {
        Toast.makeText(requireContext(), "Camera permission is required to scan number plates", Toast.LENGTH_LONG).show()
    }

    override fun onDestroyView() {
        super.onDestroyView()
        cameraExecutor?.shutdownNow()
        cameraExecutor = null
        textRecognizer.close()
        cameraProvider?.unbindAll()
        _binding = null
    }
}
