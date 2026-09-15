package com.openscansa.app.ui

import android.graphics.Bitmap
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.google.zxing.BarcodeFormat
import com.google.zxing.MultiFormatWriter
import com.lambdapioneer.argon2kt.Argon2Kt
import com.lambdapioneer.argon2kt.Argon2Mode
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentFinalizationBinding
import com.openscansa.app.utils.DateUtils
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.rpc
import io.github.jan.supabase.storage.storage
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.Serializable
import java.io.ByteArrayOutputStream
import java.security.SecureRandom
import java.util.Date
import java.util.Locale

class FinalizationFragment : Fragment() {
    private var _binding: FragmentFinalizationBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private var userRole: String? = null
    private val argon2 = Argon2Kt()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentFinalizationBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        fetchUserRole()

        binding.btnHome.setOnClickListener {
            navigateHome()
        }

        binding.btnSave.setOnClickListener {
            validateAndSave()
        }
    }

    private fun validateAndSave() {
        val passcode = binding.editPasscode.text.toString()
        val confirmPasscode = binding.editConfirmPasscode.text.toString()

        if (passcode.length != 4) {
            binding.layoutPasscode.error = "Passcode must be 4 digits"
            return
        }
        if (passcode != confirmPasscode) {
            binding.layoutConfirmPasscode.error = "Passcodes do not match"
            return
        }

        binding.layoutPasscode.error = null
        binding.layoutConfirmPasscode.error = null

        lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.btnSave.isEnabled = false
            
            try {
                // 1. Hash the passcode
                val hashedPasscode = hashPasscode(passcode)
                
                // 2. Perform the multi-table save (includes expiry checks)
                saveEverything(hashedPasscode)
                
                Toast.makeText(requireContext(), "Profile Saved Successfully!", Toast.LENGTH_LONG).show()
                navigateHome()
                
            } catch (e: Exception) {
                Log.e("OpenScanSA", "Finalization Error", e)
                val msg = e.localizedMessage ?: e.message ?: "Unknown Error"
                Toast.makeText(requireContext(), "Error: $msg", Toast.LENGTH_LONG).show()
            } finally {
                binding.progress.visibility = View.GONE
                binding.btnSave.isEnabled = true
            }
        }
    }

    private suspend fun hashPasscode(passcode: String): String = withContext(Dispatchers.Default) {
        val salt = ByteArray(16).apply { SecureRandom().nextBytes(this) }
        val result = argon2.hash(
            mode = Argon2Mode.ARGON2_ID,
            password = passcode.toByteArray(),
            salt = salt,
            tCostInIterations = 2,
            mCostInKibibyte = 32768, // 32MB
            parallelism = 1
        )
        result.encodedOutputAsString()
    }

    private suspend fun saveEverything(hashedPasscode: String) {
        val staff = scannerViewModel.pendingStaffProfile ?: throw Exception("Form details missing")
        val idData = scannerViewModel.pendingIdData
        val license = scannerViewModel.pendingLicenseData
        val vehicle = scannerViewModel.pendingVehicleData

        // 1. Fetch Network Time for final validation
        val today = DateUtils.getNetworkDate()

        // 2. Validate Expiry Dates
        if (DateUtils.isExpired(license?.validTo, today)) {
            throw Exception("VALIDATION FAILED: The Driver's License has EXPIRED.")
        }

        if (DateUtils.isExpired(vehicle?.expiryDate, today)) {
            throw Exception("VALIDATION FAILED: The Vehicle Registration has EXPIRED.")
        }

        // 3. Cross-Check ID Numbers (Security Validation)
        if (idData != null && license != null) {
            if (idData.idNumber.trim() != license.idNumber.trim()) {
                throw Exception("ID Mismatch: ID Card (${idData.idNumber}) does not match Driver's License (${license.idNumber}).")
            }
        }

        val targetIdNumber = license?.idNumber ?: idData?.idNumber ?: throw Exception("No ID/License scanned")

        // 4. Generate and Upload QR Code
        val qrUrl = try {
            val qrBytes = generateQrCodeBytes(targetIdNumber)
            val fullName = "${staff.firstNames}_${staff.lastName}"
            uploadQrCode(fullName, qrBytes)
        } catch (e: Exception) {
            throw Exception("QR Step Error: ${e.message}")
        }

        // 5. Save License Data
        license?.let {
            try {
                SupabaseManager.client.postgrest["drivers_licenses"].insert(it)
            } catch (e: Exception) {
                throw Exception("License DB Error: ${e.message}\nEnsure all columns exist in Supabase.")
            }
        }

        // 6. Save Vehicle Data
        vehicle?.let {
            try {
                SupabaseManager.client.postgrest["vehicle_registrations"].insert(it)
            } catch (e: Exception) {
                throw Exception("Vehicle DB Error: ${e.message}\nEnsure 'licence_code' column exists.")
            }
        }

        // 7. Save ID Data
        idData?.let {
            try {
                SupabaseManager.client.postgrest["ids"].upsert(it)
            } catch (e: Exception) {
                throw Exception("ID DB Error: ${e.message}\nEnsure 'is_parsed' column exists.")
            }
        }

        // 8. Save Main Profile
        try {
            val finalProfile = staff.copy(
                passcode = hashedPasscode,
                idNumber = targetIdNumber,
                licenceNumber = vehicle?.licenceNumber,
                qrCode = qrUrl
            )
            SupabaseManager.client.postgrest["profiles"].insert(finalProfile)
        } catch (e: Exception) {
            throw Exception("Profile DB Error: ${e.message}\nEnsure 'passcode', 'id_number', and 'licence_number' columns exist.")
        }
        
        // Success: Clear state
        scannerViewModel.pendingStaffProfile = null
        scannerViewModel.pendingIdData = null
        scannerViewModel.pendingLicenseData = null
        scannerViewModel.pendingVehicleData = null
    }

    private suspend fun generateQrCodeBytes(idNumber: String): ByteArray = withContext(Dispatchers.Default) {
        try {
            val payload = android.util.Base64.encodeToString(idNumber.toByteArray(), android.util.Base64.NO_WRAP)
            val writer = MultiFormatWriter()
            val bitMatrix = writer.encode(payload, BarcodeFormat.QR_CODE, 512, 512)
            val width = bitMatrix.width
            val height = bitMatrix.height
            val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565)
            for (x in 0 until width) {
                for (y in 0 until height) {
                    bitmap.setPixel(x, y, if (bitMatrix.get(x, y)) Color.BLACK else Color.WHITE)
                }
            }
            val stream = ByteArrayOutputStream()
            bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)
            stream.toByteArray()
        } catch (e: Exception) {
            throw Exception("Code Creation Failed: ${e.message}")
        }
    }

    private suspend fun uploadQrCode(name: String, bytes: ByteArray): String {
        // Sanitize filename (spaces/special chars -> underscores)
        val safeName = name.replace(Regex("[^a-zA-Z0-9]"), "_")
        val fileName = "$safeName.png"
        
        try {
            val bucket = SupabaseManager.client.storage.from("qrcodes")
            bucket.upload(path = fileName, data = bytes, upsert = true)
            return bucket.publicUrl(fileName)
        } catch (e: Exception) {
            throw Exception("Upload Failed: ${e.message}")
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
            } catch (_: Exception) {}
        }
    }

    private fun navigateHome() {
        if (userRole == "admin") {
            findNavController().navigate(R.id.adminDashboardFragment)
        } else {
            findNavController().navigate(R.id.actionHubFragment)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
