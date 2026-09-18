package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.openscansa.app.viewmodel.ScannerViewModel
import androidx.fragment.app.activityViewModels
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.databinding.FragmentAttendanceTypeBinding
import android.widget.Toast
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import com.lambdapioneer.argon2kt.Argon2Kt
import com.lambdapioneer.argon2kt.Argon2Mode
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.models.AttendanceSession
import com.openscansa.app.models.StaffRecord
import com.openscansa.app.models.AccessLogSummary
import com.openscansa.app.models.AccessLogInsert
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.query.Order
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable
import androidx.lifecycle.lifecycleScope

class AttendanceTypeFragment : Fragment() {
    private var _binding: FragmentAttendanceTypeBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private val argon2 = Argon2Kt()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentAttendanceTypeBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnBack.setOnClickListener {
            findNavController().popBackStack()
        }

        binding.btnVehicle.setOnClickListener {
            findNavController().navigate(R.id.action_attendance_type_to_vehicleEntryFragment)
        }

        binding.btnPedestrian.setOnClickListener {
            binding.containerPedestrianOptions.visibility = View.VISIBLE
            binding.btnVehicle.alpha = 0.5f
            // Button remains enabled so user can change mind if mis-clicked
        }

        binding.btnStaff.setOnClickListener {
            startFlow("pedestrian", "staff")
        }

        binding.btnVisitor.setOnClickListener {
            // Visitors will follow a different flow later
            startFlow("pedestrian", "visitor")
        }

        // Listen for scan results specifically started from this fragment
        parentFragmentManager.setFragmentResultListener("attendance_type_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyStaffMember(barcode)
            }
        }
    }

    private fun startFlow(arrivalType: String, participantType: String) {
        scannerViewModel.pendingArrivalType = arrivalType
        scannerViewModel.pendingParticipantType = participantType
        scannerViewModel.currentScanType = "in"
        
        val args = Bundle().apply {
            putBoolean("returnResult", true)
            putString("requestKey", "attendance_type_scan")
        }
        findNavController().navigate(R.id.action_attendance_type_to_scannerFragment, args)
    }

    private fun verifyStaffMember(barcode: String) {
        val decodedId = try {
            val data = android.util.Base64.decode(barcode, android.util.Base64.DEFAULT)
            String(data, Charsets.UTF_8)
        } catch (e: Exception) { barcode }

        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val profile = SupabaseManager.client.postgrest["profiles"]
                    .select {
                        filter { eq("id_number", decodedId) }
                    }.decodeSingleOrNull<StaffRecord>()

                if (profile != null) {
                    showPasscodeDialog(profile, 1)
                } else {
                    Toast.makeText(requireContext(), "User not registered ($decodedId)", Toast.LENGTH_LONG).show()
                }
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Error: ${e.message}", Toast.LENGTH_LONG).show()
            }
        }
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
                        } catch (e: Exception) { false }
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

        editReading.addTextChangedListener(object : android.text.TextWatcher {
            private var isInternalChange = false
            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {}
            override fun afterTextChanged(s: android.text.Editable?) {
                if (isInternalChange || s == null || s.isEmpty()) return
                val input = s.toString()
                val digitsOnly = input.replace(".", "")
                val formatted = if (digitsOnly.isNotEmpty()) {
                    if (digitsOnly.length == 1) digitsOnly + "." else digitsOnly.substring(0, 1) + "." + digitsOnly.substring(1)
                } else ""

                if (formatted != input) {
                    isInternalChange = true
                    s.replace(0, s.length, formatted)
                    isInternalChange = false
                }
            }
        })

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
                val reading = editReading.text.toString()

                if (passed) {
                    lifecycleScope.launch {
                        checkLastStateAndProceed(profile)
                    }
                } else {
                    val numericReading = reading.toDoubleOrNull() ?: 0.0
                    if (reading.isBlank()) {
                        Toast.makeText(requireContext(), "Please enter a reading for failed test.", Toast.LENGTH_SHORT).show()
                        showBreathalyzerDialog(profile) // Reshow
                    } else if (reading.length < 4) {
                        Toast.makeText(requireContext(), "Please enter the full 3-digit reading (e.g., 0.15).", Toast.LENGTH_SHORT).show()
                        showBreathalyzerDialog(profile)
                    } else if (numericReading <= 0.0) {
                        MaterialAlertDialogBuilder(requireContext())
                            .setTitle("Invalid Reading")
                            .setMessage("A $reading reading is a PASS, not a FAIL. Please enter a valid failing result greater than 0.00.")
                            .setPositiveButton("OK") { _, _ -> showBreathalyzerDialog(profile) }
                            .show()
                    } else {
                        logBreathalyzerFailure(profile, reading)
                    }
                }
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun logBreathalyzerFailure(profile: StaffRecord, reading: String) {
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val logEntry = AccessLogInsert(
                    profileId = profile.id,
                    scanType = "denied_access",
                    reissueQr = profile.needsNewQr,
                    documentType = profile.documentTypeUsed
                )
                SupabaseManager.client.postgrest["access_logs"].insert(logEntry)
                
                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("ACCESS DENIED")
                    .setMessage("USER FAILED BREATHALYZER TEST (Reading: $reading). ENTRY DENIED.")
                    .setPositiveButton("OK", null)
                    .show()
            } catch (e: Exception) {
                val errorMsg = e.localizedMessage ?: e.message ?: "Unknown"
                Toast.makeText(requireContext(), "Logging Error: $errorMsg", Toast.LENGTH_LONG).show()
            }
        }
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
            val pendingScanType = "in" // Selection screen is only for Scan In
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
        } catch (e: Exception) {
            Toast.makeText(requireContext(), "State Check Error: ${e.message}", Toast.LENGTH_LONG).show()
        }
    }

    private fun startAttendanceSession(profile: StaffRecord, warning: String?) {
        val arrivalType = scannerViewModel.pendingArrivalType ?: "vehicle"
        val participantType = scannerViewModel.pendingParticipantType ?: "staff"

        val record = profile.apply { 
            missingStateWarning = warning
            needsNewQr = false
            documentTypeUsed = null // Direct QR scan
        }

        scannerViewModel.attendanceSession = AttendanceSession(
            scanType = "in",
            arrivalType = arrivalType,
            participantType = participantType,
            driver = record
        )

        scannerViewModel.pendingArrivalType = null
        scannerViewModel.pendingParticipantType = null

        if (arrivalType == "vehicle") {
            findNavController().navigate(R.id.attendanceVehiclePromptFragment)
        } else {
            findNavController().navigate(R.id.attendanceSummaryFragment)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
