package com.openscansa.app.ui

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.fragment.app.activityViewModels
import androidx.navigation.fragment.findNavController
import com.google.android.material.card.MaterialCardView
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import com.lambdapioneer.argon2kt.Argon2Kt
import com.lambdapioneer.argon2kt.Argon2Mode
import com.openscansa.app.MainActivity
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentActionHubBinding
import com.openscansa.app.models.AttendanceSession
import com.openscansa.app.models.StaffRecord
import com.openscansa.app.models.AccessLog
import com.openscansa.app.models.AccessLogInsert
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.query.Order
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable
import java.text.SimpleDateFormat
import java.util.Locale
import java.util.TimeZone

class ActionHubFragment : Fragment() {
    private var _binding: FragmentActionHubBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: com.openscansa.app.viewmodel.ScannerViewModel by activityViewModels()
    private val argon2 = Argon2Kt()
    private var pendingScanType: String = "in"

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentActionHubBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        fetchUserData()
        fetchAttendanceHistory()

        // Listen for verification scan result
        parentFragmentManager.setFragmentResultListener("action_hub_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyStaffMember(barcode)
            }
        }

        binding.btnScanIn.setOnClickListener {
            pendingScanType = "in"
            scannerViewModel.currentScanType = "in"
            findNavController().navigate(R.id.action_actionHubFragment_to_attendance_type)
        }

        binding.btnScanOut.setOnClickListener {
            pendingScanType = "out"
            scannerViewModel.currentScanType = "out"
            openScanner()
        }

        binding.btnAddProfile.setOnClickListener {
            findNavController().navigate(R.id.fragmentAddProfile)
        }

        binding.btnHome.setOnClickListener {
            findNavController().navigate(R.id.adminDashboardFragment)
        }

        binding.btnSignOut.setOnClickListener {
            viewLifecycleOwner.lifecycleScope.launch {
                try {
                    SupabaseManager.client.auth.signOut()
                } catch (_: Exception) {}
                
                val intent = Intent(requireContext(), MainActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                startActivity(intent)
            }
        }

        binding.btnBack.setOnClickListener {
            findNavController().popBackStack()
        }
    }

    private fun openScanner() {
        val args = Bundle().apply {
            putBoolean("returnResult", true)
            putString("requestKey", "action_hub_scan")
        }
        findNavController().navigate(R.id.scannerFragment, args)
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
                    showError("User not registered ($decodedId)")
                }
            } catch (e: Exception) {
                showError("Error: ${e.message}")
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
                        // Breathalyzer only for Scan In
                        if (pendingScanType == "in") {
                            showBreathalyzerDialog(profile)
                        } else {
                            checkLastStateAndProceed(profile)
                        }
                    } else if (attempt < 3) {
                        showPasscodeDialog(profile, attempt + 1)
                    } else {
                        showError("Invalid Passcode")
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
                    showError("Please select a test result.")
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
                        showError("Please enter a reading for failed test.")
                        showBreathalyzerDialog(profile)
                    } else if (reading.length < 4) {
                        showError("Please enter the full 3-digit reading (e.g., 0.15).")
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
                // Use the new AccessLogInsert serializable class
                val logEntry = AccessLogInsert(
                    profileId = profile.id,
                    scanType = "denied_access",
                    reissueQr = false,
                    documentType = null
                )
                SupabaseManager.client.postgrest["access_logs"].insert(logEntry)
                
                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("ACCESS DENIED")
                    .setMessage("USER FAILED BREATHALYZER TEST (Reading: $reading). ENTRY DENIED.")
                    .setPositiveButton("OK", null)
                    .show()
                    
                fetchAttendanceHistory() // Refresh UI
            } catch (e: Exception) {
                val errorMsg = e.localizedMessage ?: e.message ?: "Unknown"
                showError("Logging Error: $errorMsg")
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
                startAttendanceSession(profile, null)
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
                val warning = if (pendingScanType == "in") "missing_out" else "missing_in"
                startAttendanceSession(profile, warning)
            }
            .setNegativeButton("Cancel", null)
            .show()
    }

    private fun startAttendanceSession(profile: StaffRecord, warning: String?) {
        val arrivalType = scannerViewModel.pendingArrivalType ?: "vehicle"
        val participantType = scannerViewModel.pendingParticipantType ?: "staff"

        scannerViewModel.attendanceSession = AttendanceSession(
            scanType = pendingScanType,
            arrivalType = arrivalType,
            participantType = participantType,
            driver = profile.apply { missingStateWarning = warning }
        )

        // Clear pending states
        scannerViewModel.pendingArrivalType = null
        scannerViewModel.pendingParticipantType = null

        if (arrivalType == "vehicle") {
            findNavController().navigate(R.id.attendanceVehiclePromptFragment)
        } else {
            // Pedestrians go straight to summary
            findNavController().navigate(R.id.attendanceSummaryFragment)
        }
    }

    private fun fetchAttendanceHistory() {
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                // Fetch logs
                val logs = SupabaseManager.client.postgrest["access_logs"]
                    .select {
                        order("scan_time", Order.DESCENDING)
                        limit(20)
                    }.decodeList<AccessLog>()
                
                if (logs.isEmpty()) {
                    updateLogsUi(emptyList())
                    return@launch
                }

                // Get unique profile IDs from logs
                val profileIds = logs.map { it.profileId }.distinct()
                
                // Fetch profiles for these IDs to get names
                val profiles = SupabaseManager.client.postgrest["profiles"]
                    .select {
                        filter {
                            // Using list of IDs
                            or {
                                profileIds.forEach { id ->
                                    eq("id", id)
                                }
                            }
                        }
                    }.decodeList<StaffRecord>()
                
                val profileMap = profiles.associateBy { it.id }

                // Fetch latest log for each unique profile to determine current QR status
                val latestLogs = SupabaseManager.client.postgrest["access_logs"]
                    .select {
                        filter {
                            or {
                                profileIds.forEach { id ->
                                    eq("profile_id", id)
                                }
                            }
                            neq("scan_type", "denied_access")
                        }
                        order("scan_time", Order.DESCENDING)
                    }.decodeList<AccessLog>()
                
                val currentStatusMap = latestLogs.groupBy { it.profileId }
                    .mapValues { (_, userLogs) -> 
                        // Find the most recent log that changes QR state:
                        // Either a 'Lost QR' scan (reissueQr = true) or a normal 'QR Scan' (documentType = null)
                        val statusLog = userLogs.firstOrNull { it.reissueQr || it.documentType == null }
                        statusLog?.reissueQr ?: false 
                    }
                
                // Update logs with names from profiles and current QR status
                val enrichedLogs = logs.map { log ->
                    val profile = profileMap[log.profileId]
                    log.copy(
                        firstNames = profile?.firstNames,
                        lastName = profile?.lastName,
                    ).apply {
                        currentNeedsReissue = currentStatusMap[log.profileId] ?: false
                    }
                }
                
                updateLogsUi(enrichedLogs)
            } catch (e: Exception) {
                // Log history fetch errors for debugging
                android.util.Log.e("OpenScanSA", "History Error: ${e.message}")
            }
        }
    }

    private fun updateLogsUi(logs: List<AccessLog>) {
        binding.containerLogs.removeAllViews()
        for (log in logs) {
            binding.containerLogs.addView(createLogView(log))
        }
    }

    private fun createLogView(log: AccessLog): View {
        val card = MaterialCardView(requireContext()).apply {
            layoutParams = LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT).apply { setMargins(0, 0, 0, 12) }
            radius = 12f
            setCardBackgroundColor(requireContext().getColor(R.color.scrim))
            strokeWidth = 0
            elevation = 0f
        }
        val layout = LinearLayout(requireContext()).apply { orientation = LinearLayout.VERTICAL; setPadding(16, 16, 16, 16) }
        val nameTv = TextView(requireContext()).apply {
            text = "${log.firstNames} ${log.lastName}"
            setTextColor(requireContext().getColor(R.color.white))
            textSize = 16f
            setTypeface(null, android.graphics.Typeface.BOLD)
        }
        val statusTv = TextView(requireContext()).apply {
            val status = when (log.scanType) {
                "in" -> "SCANNED IN"
                "out" -> "SCANNED OUT"
                "missing_out" -> "WARNING: MISSING OUT"
                "missing_in" -> "WARNING: MISSING IN"
                "denied_access" -> "DENIED ACCESS"
                else -> log.scanType.uppercase()
            }
            val time = formatServerTime(log.scanTime)
            val date = formatServerDate(log.scanTime)
            text = "$status at $time on $date"
            val color = when (log.scanType) {
                "in" -> R.color.accent_green
                "out" -> R.color.warning_amber
                "denied_access" -> R.color.primary_red
                else -> R.color.blue_grey
            }
            setTextColor(requireContext().getColor(color))
            textSize = 14f
            setPadding(0, 4, 0, 0)
        }

        layout.addView(nameTv)
        layout.addView(statusTv)

        // Banner Logic: Both can be displayed concurrently
        if (log.reissueQr && log.currentNeedsReissue) {
            // Priority 1: User still needs a new QR code (Blue)
            val reissueTv = TextView(requireContext()).apply {
                text = "User must be issued new QR code"
                setTextColor(requireContext().getColor(R.color.accent_blue))
                textSize = 12f
                setTypeface(null, android.graphics.Typeface.ITALIC)
                setPadding(0, 4, 0, 0)
            }
            layout.addView(reissueTv)
        } 
        
        if (log.documentType != null) {
            // Priority 2: User used a document (Pink)
            val docTv = TextView(requireContext()).apply {
                val action = if (log.scanType == "in" || log.scanType == "missing_out") "entered" else "exited"
                text = "User $action using ${log.documentType}"
                setTextColor(requireContext().getColor(R.color.accent_pink))
                textSize = 12f
                setTypeface(null, android.graphics.Typeface.ITALIC)
                setPadding(0, 4, 0, 0)
            }
            layout.addView(docTv)
        }

        card.addView(layout)
        return card
    }

    private fun formatServerTime(isoString: String?): String {
        if (isoString == null) return "Unknown"
        return try {
            val parser = SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.getDefault())
            parser.timeZone = TimeZone.getTimeZone("UTC")
            val date = parser.parse(isoString) ?: return isoString
            SimpleDateFormat("HH:mm:ss", Locale.getDefault()).format(date)
        } catch (_: Exception) { isoString.substringAfter("T").substringBefore(".") }
    }

    private fun formatServerDate(isoString: String?): String {
        if (isoString == null) return ""
        return try {
            val parser = SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.getDefault())
            parser.timeZone = TimeZone.getTimeZone("UTC")
            val date = parser.parse(isoString) ?: return isoString
            SimpleDateFormat("dd MMMM, yyyy", Locale.getDefault()).format(date)
        } catch (_: Exception) { "" }
    }

    private fun showError(message: String) { Toast.makeText(requireContext(), message, Toast.LENGTH_LONG).show() }

    private fun fetchUserData() {
        val user = SupabaseManager.client.auth.currentUserOrNull() ?: return
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val profile = SupabaseManager.client.postgrest["users"].select { filter { eq("id", user.id) } }.decodeSingleOrNull<UserProfile>()
                if (profile != null && !profile.full_name.isNullOrBlank()) binding.greetingText.text = "Hello, ${profile.full_name}"
                val whitelist = SupabaseManager.client.postgrest["allowed_emails"].select { filter { eq("email", user.email ?: "") } }.decodeSingleOrNull<WhitelistEntry>()
                if (whitelist?.role == "admin") {
                    binding.btnBack.visibility = View.VISIBLE
                    binding.btnHome.visibility = View.VISIBLE
                } else {
                    binding.btnBack.visibility = View.GONE
                    binding.btnHome.visibility = View.GONE
                }
            } catch (_: Exception) {}
        }
    }

    override fun onDestroyView() { super.onDestroyView(); _binding = null }

    @Serializable data class UserProfile(val id: String, val full_name: String?)
    @Serializable data class WhitelistEntry(val email: String, val role: String?)
}
