package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import com.lambdapioneer.argon2kt.Argon2Kt
import com.lambdapioneer.argon2kt.Argon2Mode
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentScanMethodBinding
import com.openscansa.app.models.AttendanceSession
import com.openscansa.app.models.StaffRecord
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.query.Order
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable
import java.util.UUID

class ScanMethodFragment : Fragment() {
    private var _binding: FragmentScanMethodBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private val argon2 = Argon2Kt()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentScanMethodBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnBack.setOnClickListener {
            findNavController().popBackStack()
        }

        binding.btnScanQr.setOnClickListener {
            navigateToScanner("scan_qr_code")
        }

        binding.btnScanId.setOnClickListener {
            navigateToScanner("scan_id")
        }

        binding.btnScanLicense.setOnClickListener {
            navigateToScanner("scan_license")
        }

        parentFragmentManager.setFragmentResultListener("scan_qr_code", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyDriver(barcode, "QR")
            }
        }

        parentFragmentManager.setFragmentResultListener("scan_id", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyDriver(barcode, "ID")
            }
        }

        parentFragmentManager.setFragmentResultListener("scan_license", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyDriver(barcode, "Licence")
            }
        }
    }

    private fun navigateToScanner(requestKey: String) {
        val args = Bundle().apply {
            putBoolean("returnResult", true)
            putString("requestKey", requestKey)
        }
        findNavController().navigate(R.id.scannerFragment, args)
    }

    private fun verifyDriver(barcode: String, scanType: String) {
        val decodedId = when (scanType) {
            "QR" -> {
                try {
                    val data = android.util.Base64.decode(barcode, android.util.Base64.DEFAULT)
                    String(data, Charsets.UTF_8)
                } catch (_: Exception) { barcode }
            }
            "ID" -> {
                if (barcode.contains("|")) {
                    barcode.split("|").getOrNull(4) ?: barcode
                } else {
                    barcode
                }
            }
            "Licence" -> {
                scannerViewModel.pendingLicenseData?.idNumber ?: barcode
            }
            else -> barcode
        }.trim()

        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val profile = SupabaseManager.client.postgrest["profiles"]
                    .select {
                        filter { 
                            or {
                                eq("id_number", decodedId)
                                eq("licence_number", decodedId)
                            }
                        }
                    }.decodeSingleOrNull<StaffRecord>()

                if (profile != null) {
                    profile.documentTypeUsed = when(scanType) {
                        "ID" -> "ID Document"
                        "Licence" -> "Driver's Licence"
                        else -> null
                    }
                    showPasscodeDialog(profile, 1)
                } else {
                    handleUnregisteredDriver(barcode, scanType, decodedId)
                }
            } catch (_: Exception) {
                handleUnregisteredDriver(barcode, scanType, decodedId)
            }
        }
    }

    private fun handleUnregisteredDriver(barcode: String, scanType: String, identifier: String) {
        val (firstName, lastName) = when (scanType) {
            "ID" -> {
                if (barcode.contains("|")) {
                    val parts = barcode.split("|")
                    Pair(parts.getOrNull(1) ?: "Guest", parts.getOrNull(0) ?: "Driver")
                } else {
                    Pair("Unregistered", "Driver")
                }
            }
            "Licence" -> {
                val lic = scannerViewModel.pendingLicenseData
                Pair(lic?.initials ?: "Unregistered", lic?.surname ?: "Driver")
            }
            else -> Pair("Unregistered", "Driver")
        }

        val unregisteredRecord = StaffRecord(
            id = UUID.randomUUID().toString(),
            firstNames = firstName,
            lastName = lastName,
            idNumber = identifier,
            passcode = null,
            documentTypeUsed = when(scanType) {
                "ID" -> "Unregistered ID Document"
                "Licence" -> "Unregistered Driver's Licence"
                else -> "Unregistered QR"
            }
        )

        startAttendanceSession(unregisteredRecord, null)
    }

    private fun showPasscodeDialog(profile: StaffRecord, attempt: Int) {
        val dialogView = LayoutInflater.from(requireContext()).inflate(R.layout.dialog_passcode_entry, null)
        val editPasscode = dialogView.findViewById<TextInputEditText>(R.id.edit_passcode)
        val layoutPasscode = dialogView.findViewById<TextInputLayout>(R.id.layout_passcode)
        val btnForgot = dialogView.findViewById<View>(R.id.btn_forgot_passcode)
        
        if (attempt > 1) layoutPasscode.error = "Incorrect. Attempt $attempt of 3"

        val dialog = MaterialAlertDialogBuilder(requireContext())
            .setView(dialogView)
            .setCancelable(false)
            .setPositiveButton("Verify") { _, _ ->
                val entered = editPasscode.text.toString()
                lifecycleScope.launch {
                    val isValid = withContext(Dispatchers.Default) {
                        try {
                            argon2.verify(mode = Argon2Mode.ARGON2_ID, encoded = profile.passcode ?: "", password = entered.toByteArray())
                        } catch (_: Exception) { false }
                    }

                    if (isValid) {
                        showBreathalyzerDialog(profile)
                    } else if (attempt < 3) {
                        showPasscodeDialog(profile, attempt + 1)
                    } else {
                        Toast.makeText(requireContext(), "Invalid Passcode", Toast.LENGTH_LONG).show()
                    }
                }
            }
            .setNegativeButton("Cancel", null)
            .create()

        btnForgot.setOnClickListener {
            dialog.dismiss()
            findNavController().navigate(R.id.forgotPasscodeFragment)
        }

        dialog.show()
    }

    private fun showBreathalyzerDialog(profile: StaffRecord) {
        val dialogView = LayoutInflater.from(requireContext()).inflate(R.layout.dialog_breathalyzer, null)
        val radioGroup = dialogView.findViewById<android.widget.RadioGroup>(R.id.radio_group_result)
        val layoutReading = dialogView.findViewById<TextInputLayout>(R.id.layout_reading)
        val editReading = dialogView.findViewById<TextInputEditText>(R.id.edit_reading)

        radioGroup.setOnCheckedChangeListener { _, checkedId ->
            layoutReading.visibility = if (checkedId == R.id.radio_fail) View.VISIBLE else View.GONE
        }

        MaterialAlertDialogBuilder(requireContext())
            .setView(dialogView)
            .setCancelable(false)
            .setPositiveButton("Confirm") { _, _ ->
                val checkedId = radioGroup.checkedRadioButtonId
                if (checkedId == -1) {
                    Toast.makeText(requireContext(), "Please select a test result.", Toast.LENGTH_SHORT).show()
                    showBreathalyzerDialog(profile)
                    return@setPositiveButton
                }

                val passed = checkedId == R.id.radio_pass
                if (passed) {
                    lifecycleScope.launch {
                        checkLastStateAndProceed(profile)
                    }
                } else {
                    Toast.makeText(requireContext(), "Breathalyzer test failed.", Toast.LENGTH_LONG).show()
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private suspend fun checkLastStateAndProceed(profile: StaffRecord) {
        try {
            val lastLog = SupabaseManager.client.postgrest["access_logs"]
                .select {
                    filter { 
                        eq("profile_id", profile.id)
                        neq("scan_type", "denied_access")
                    }
                    order("scan_time", Order.DESCENDING)
                    limit(1)
                }.decodeSingleOrNull<AccessLogSummary>()

            val lastType = lastLog?.scanType
            val isMismatch = (lastType == "in")

            if (isMismatch) {
                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("State Mismatch")
                    .setMessage("User has not been scanned out. Proceed anyway?")
                    .setPositiveButton("Proceed") { _, _ ->
                        startAttendanceSession(profile, "missing_out")
                    }
                    .setNegativeButton("Cancel", null)
                    .show()
            } else {
                startAttendanceSession(profile, null)
            }
        } catch (_: Exception) {
            startAttendanceSession(profile, null)
        }
    }

    private fun startAttendanceSession(profile: StaffRecord, warning: String?) {
        val arrivalType = scannerViewModel.pendingArrivalType ?: "pedestrian"
        val participantType = scannerViewModel.pendingParticipantType ?: "staff"

        profile.missingStateWarning = warning

        if (scannerViewModel.attendanceSession == null) {
            scannerViewModel.attendanceSession = AttendanceSession(
                scanType = "in",
                arrivalType = arrivalType,
                participantType = participantType,
                vehicle = scannerViewModel.pendingVehicleData
            )
        }
        
        scannerViewModel.attendanceSession?.driver = profile

        scannerViewModel.pendingArrivalType = null
        scannerViewModel.pendingParticipantType = null
        scannerViewModel.pendingVehicleData = null

        findNavController().navigate(R.id.attendanceSummaryFragment)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class AccessLogSummary(@SerialName("scan_type") val scanType: String)
}
