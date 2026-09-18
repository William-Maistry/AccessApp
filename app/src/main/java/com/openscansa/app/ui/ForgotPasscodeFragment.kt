package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
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
import com.openscansa.app.databinding.FragmentForgotPasscodeBinding
import com.openscansa.app.models.*
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.query.Order
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.security.SecureRandom

class ForgotPasscodeFragment : Fragment() {
    private var _binding: FragmentForgotPasscodeBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private val argon2 = Argon2Kt()
    private var verifiedProfile: StaffRecord? = null
    private var currentDocType: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentForgotPasscodeBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupListeners()
        setupResultListeners()
    }

    private fun setupListeners() {
        binding.btnHome.setOnClickListener {
            findNavController().popBackStack(R.id.actionHubFragment, false)
        }

        binding.btnScanId.setOnClickListener {
            val args = Bundle().apply {
                putBoolean("returnResult", true)
                putString("requestKey", "forgot_id_scan")
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnScanLicense.setOnClickListener {
            val args = Bundle().apply {
                putBoolean("returnResult", true)
                putString("requestKey", "forgot_license_scan")
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnConfirmRecovery.setOnClickListener {
            handleReset()
        }
    }

    private fun setupResultListeners() {
        parentFragmentManager.setFragmentResultListener("forgot_id_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                currentDocType = "ID Card"
                val idData = parseIdBarcode(barcode)
                displayIdResult(idData)
                verifyAgainstDatabase(idData)
            }
        }

        parentFragmentManager.setFragmentResultListener("forgot_license_scan", viewLifecycleOwner) { _, bundle ->
            val hasLicense = bundle.getBoolean("hasLicenseInfo")
            if (hasLicense) {
                currentDocType = "Driver's License"
                val licenseData = scannerViewModel.pendingLicenseData
                if (licenseData != null) {
                    displayLicenseResult(licenseData)
                    verifyAgainstDatabase(licenseData)
                }
            }
        }
    }

    private fun parseIdBarcode(result: String): IdData {
        return if (result.contains("|")) {
            val parts = result.split("|")
            IdData(
                surname = parts.getOrNull(0),
                firstNames = parts.getOrNull(1),
                sex = parts.getOrNull(2),
                nationality = parts.getOrNull(3),
                idNumber = parts.getOrNull(4) ?: "UNKNOWN",
                dob = parts.getOrNull(5),
                citizenshipCountry = parts.getOrNull(6),
                citizenType = parts.getOrNull(7),
                cardIssueDate = parts.getOrNull(8),
                issuingOfficeCode = parts.getOrNull(9),
                internalRecordNumber = parts.getOrNull(10),
                isParsed = true,
                rawData = result
            )
        } else {
            IdData(idNumber = result, isParsed = false, rawData = result)
        }
    }

    private fun verifyAgainstDatabase(data: Any) {
        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                val idNumber = when (data) {
                    is IdData -> data.idNumber
                    is LicenseData -> data.idNumber
                    else -> ""
                }

                val profile = SupabaseManager.client.postgrest["profiles"]
                    .select { filter { eq("id_number", idNumber) } }
                    .decodeSingleOrNull<StaffRecord>()

                if (profile == null) {
                    showError("Access Denied: No profile found for ID $idNumber.")
                    return@launch
                }

                val isMatch = if (data is IdData) {
                    validateIdMatch(data, profile)
                } else if (data is LicenseData) {
                    validateLicenseMatch(data, profile)
                } else false

                if (isMatch) {
                    verifiedProfile = profile
                    binding.containerRecoveryActions.visibility = View.VISIBLE
                    Toast.makeText(requireContext(), "Identity Verified Successfully", Toast.LENGTH_SHORT).show()
                } else {
                    showError("Verification Failed: Document details do not match our records.")
                    binding.containerRecoveryActions.visibility = View.GONE
                }

            } catch (e: Exception) {
                showError("System Error: ${e.message}")
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun validateIdMatch(scanned: IdData, db: StaffRecord): Boolean {
        if (!scanned.isParsed) return true
        val sSurname = scanned.surname?.trim()?.uppercase() ?: ""
        val dSurname = db.lastName.trim().uppercase()
        val sNames = scanned.firstNames?.trim()?.uppercase() ?: ""
        val dNames = db.firstNames.trim().uppercase()
        return sSurname == dSurname && dNames.contains(sNames.split(" ")[0])
    }

    private fun validateLicenseMatch(scanned: LicenseData, db: StaffRecord): Boolean {
        val sSurname = scanned.surname?.trim()?.uppercase() ?: ""
        val dSurname = db.lastName.trim().uppercase()
        val sInitials = scanned.initials?.trim()?.uppercase() ?: ""
        val dInitials = db.firstNames.trim().split(" ").joinToString("") { it.take(1).uppercase() }
        return sSurname == dSurname && (sInitials == dInitials || db.firstNames.uppercase().startsWith(sInitials))
    }

    private fun handleReset() {
        val profile = verifiedProfile ?: return
        val newPass = binding.editNewPasscode.text.toString()
        val confirmPass = binding.editConfirmPasscode.text.toString()

        if (newPass.length != 4) {
            Toast.makeText(requireContext(), "Passcode must be 4 digits", Toast.LENGTH_SHORT).show()
            return
        }
        if (newPass != confirmPass) {
            Toast.makeText(requireContext(), "Passcodes do not match", Toast.LENGTH_SHORT).show()
            return
        }

        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                val hashed = hashPasscode(newPass)
                SupabaseManager.client.postgrest["profiles"]
                    .update({
                        set("passcode", hashed)
                    }) {
                        filter { eq("id", profile.id) }
                    }
                
                Toast.makeText(requireContext(), "Passcode Reset Successfully", Toast.LENGTH_LONG).show()
                proceedToAttendance(profile)
            } catch (e: Exception) {
                showError("Reset Error: ${e.message}")
            } finally {
                binding.progress.visibility = View.GONE
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
            mCostInKibibyte = 32768,
            parallelism = 1
        )
        result.encodedOutputAsString()
    }

    private fun proceedToAttendance(profile: StaffRecord) {
        viewLifecycleOwner.lifecycleScope.launch {
            checkLastStateAndProceed(profile)
        }
    }

    private suspend fun checkLastStateAndProceed(profile: StaffRecord) {
        val pendingScanType = scannerViewModel.currentScanType
        try {
            val lastLog = SupabaseManager.client.postgrest["access_logs"]
                .select {
                    filter {
                        eq("profile_id", profile.id)
                        neq("scan_type", "denied_access")
                    }
                    order("scan_time", Order.DESCENDING)
                    limit(1)
                }.decodeSingleOrNull<AccessLog>()

            val lastType = lastLog?.scanType
            val isMismatch = (pendingScanType == "in" && lastType == "in") || 
                             (pendingScanType == "out" && (lastType == "out" || lastType == null))

            if (isMismatch) {
                val message = if (pendingScanType == "in") 
                    "User has not been scanned out. Proceed anyway?" 
                else 
                    "User has not been scanned in. Proceed anyway?"
                
                showMismatchAlert(profile, message)
            } else {
                completeAttendanceSession(profile, null)
            }
        } catch (e: Exception) {
            showError("State Check Error: ${e.message}")
        }
    }

    private fun showMismatchAlert(profile: StaffRecord, message: String) {
        MaterialAlertDialogBuilder(requireContext())
            .setTitle("State Mismatch")
            .setMessage(message)
            .setPositiveButton("Proceed") { _, _ ->
                val warning = if (scannerViewModel.currentScanType == "in") "missing_out" else "missing_in"
                completeAttendanceSession(profile, warning)
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun completeAttendanceSession(profile: StaffRecord, warning: String?) {
        val scanType = scannerViewModel.currentScanType
        val reqKey = arguments?.getString("requestKey") ?: "scan_request"
        
        val session = scannerViewModel.attendanceSession ?: AttendanceSession(scanType = scanType)
        
        val record = profile.apply { 
            missingStateWarning = warning
            needsNewQr = false
            documentTypeUsed = currentDocType
        }

        if (reqKey == "passenger_scan") {
            session.arrivalType = "vehicle"
            session.passengers.add(record)
        } else {
            session.driver = record
        }
        
        scannerViewModel.attendanceSession = session

        if (scanType == "in") {
            showBreathalyzerDialog(profile)
        } else {
            findNavController().navigate(R.id.attendanceSummaryFragment)
        }
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
                    findNavController().navigate(R.id.attendanceSummaryFragment)
                } else {
                    val numericReading = reading.toDoubleOrNull() ?: 0.0
                    if (reading.isBlank()) {
                        Toast.makeText(requireContext(), "Please enter a reading.", Toast.LENGTH_SHORT).show()
                        showBreathalyzerDialog(profile)
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
            .setNegativeButton("Cancel") { _, _ ->
                findNavController().popBackStack(R.id.actionHubFragment, false)
            }
            .show()
    }

    private fun logBreathalyzerFailure(profile: StaffRecord, reading: String) {
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val data = AccessLogInsert(
                    profileId = profile.id,
                    scanType = "denied_access",
                    reissueQr = profile.needsNewQr,
                    documentType = profile.documentTypeUsed
                )
                SupabaseManager.client.postgrest["access_logs"].insert(data)
                
                val reqKey = arguments?.getString("requestKey") ?: "scan_request"
                val isPassenger = reqKey == "passenger_scan"

                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("ACCESS DENIED")
                    .setMessage("USER FAILED BREATHALYZER TEST (Reading: $reading). ENTRY DENIED.")
                    .setPositiveButton("OK") { _, _ ->
                        if (isPassenger) {
                            scannerViewModel.attendanceSession?.passengers?.remove(profile)
                            findNavController().navigate(R.id.attendanceSummaryFragment)
                        } else {
                            findNavController().popBackStack(R.id.actionHubFragment, false)
                        }
                    }
                    .show()
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Logging Error: ${e.message}", Toast.LENGTH_LONG).show()
            }
        }
    }

    private fun displayIdResult(data: IdData) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE
        if (data.isParsed) {
            addField("Surname", data.surname)
            addField("First Names", data.firstNames)
            addField("ID Number", data.idNumber)
        } else {
            addField("ID Number", data.idNumber)
        }
    }

    private fun displayLicenseResult(data: LicenseData) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE
        addField("Surname", data.surname)
        addField("Initials", data.initials)
        addField("License Number", data.licenseNumber)
        addField("ID Number", data.idNumber)
    }

    private fun addField(label: String, value: String?) {
        if (value.isNullOrBlank()) return
        val row = LinearLayout(requireContext()).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(0, 0, 0, 16)
        }
        val l = TextView(requireContext()).apply { text = label; setTextColor(requireContext().getColor(R.color.grey)); textSize = 12f }
        val v = TextView(requireContext()).apply { text = value; setTextColor(requireContext().getColor(R.color.white)); textSize = 18f }
        row.addView(l)
        row.addView(v)
        binding.containerResults.addView(row)
    }

    private fun showError(msg: String) {
        MaterialAlertDialogBuilder(requireContext())
            .setTitle("Error")
            .setMessage(msg)
            .setPositiveButton("Retry", null)
            .setNegativeButton("Home") { _, _ -> findNavController().popBackStack(R.id.actionHubFragment, false) }
            .show()
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
