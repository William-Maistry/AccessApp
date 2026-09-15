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
import com.google.android.material.card.MaterialCardView
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import com.lambdapioneer.argon2kt.Argon2Kt
import com.lambdapioneer.argon2kt.Argon2Mode
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentAttendanceSummaryBinding
import com.openscansa.app.models.AttendanceSession
import com.openscansa.app.models.StaffRecord
import com.openscansa.app.models.AccessLogSummary
import com.openscansa.app.models.AccessLogInsert
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.query.Order
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

class AttendanceSummaryFragment : Fragment() {
    private var _binding: FragmentAttendanceSummaryBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private val argon2 = Argon2Kt()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentAttendanceSummaryBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        updateUi()

        binding.btnHome.setOnClickListener {
            scannerViewModel.attendanceSession = null
            findNavController().popBackStack(R.id.actionHubFragment, false)
        }

        binding.btnAddPassenger.setOnClickListener {
            // Loop back to scanner for next passenger
            val args = Bundle().apply {
                putBoolean("returnResult", true)
                putString("requestKey", "passenger_scan")
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnFinalize.setOnClickListener {
            saveSession()
        }

        // Listen for passenger scan result
        parentFragmentManager.setFragmentResultListener("passenger_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                verifyPassenger(barcode)
            }
        }
    }

    private fun verifyPassenger(barcode: String) {
        val decodedId = try {
            val data = android.util.Base64.decode(barcode, android.util.Base64.DEFAULT)
            String(data, Charsets.UTF_8)
        } catch (_: Exception) { barcode }

        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                val profile = SupabaseManager.client.postgrest["profiles"]
                    .select {
                        filter { eq("id_number", decodedId) }
                    }.decodeSingleOrNull<StaffRecord>()

                if (profile != null) {
                    showPasscodeDialog(profile)
                } else {
                    Toast.makeText(requireContext(), "User not registered ($decodedId)", Toast.LENGTH_LONG).show()
                }
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Error: ${e.message}", Toast.LENGTH_LONG).show()
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun showPasscodeDialog(profile: StaffRecord, attempt: Int = 1) {
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
                        // Breathalyzer only for Scan In
                        if (scannerViewModel.attendanceSession?.scanType == "in") {
                            showBreathalyzerDialog(profile)
                        } else {
                            checkLastStateAndAdd(profile)
                        }
                    } else if (attempt < 3) {
                        showPasscodeDialog(profile, attempt + 1)
                    } else {
                        profile.missingStateWarning = "Invalid Passcode"
                        addPassengerToSession(profile)
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
                        checkLastStateAndAdd(profile)
                    }
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
                    documentType = null // Passengers use QR codes
                )
                SupabaseManager.client.postgrest["access_logs"].insert(logEntry)
                
                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("ACCESS DENIED")
                    .setMessage("PASSENGER FAILED BREATHALYZER TEST (Reading: $reading). DENIED.")
                    .setPositiveButton("OK", null)
                    .show()
            } catch (e: Exception) {
                val errorMsg = e.localizedMessage ?: e.message ?: "Unknown"
                Toast.makeText(requireContext(), "Logging Error: $errorMsg", Toast.LENGTH_LONG).show()
            }
        }
    }

    private suspend fun checkLastStateAndAdd(profile: StaffRecord) {
        val session = scannerViewModel.attendanceSession ?: return
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
            val isMismatch = (session.scanType == "in" && lastType == "in") || 
                             (session.scanType == "out" && (lastType == "out" || lastType == null))

            if (isMismatch) {
                val msg = if (session.scanType == "in") "User not scanned out." else "User not scanned in."
                MaterialAlertDialogBuilder(requireContext())
                    .setTitle("State Mismatch")
                    .setMessage("$msg Proceed anyway?")
                    .setPositiveButton("Proceed") { _, _ ->
                        profile.missingStateWarning = if (session.scanType == "in") "missing_out" else "missing_in"
                        addPassengerToSession(profile)
                    }
                    .setNegativeButton("Cancel", null)
                    .show()
            } else {
                addPassengerToSession(profile)
            }
        } catch (_: Exception) {
            addPassengerToSession(profile)
        }
    }

    private fun addPassengerToSession(profile: StaffRecord) {
        scannerViewModel.attendanceSession?.passengers?.add(profile)
        updateUi()
    }

    private fun updateUi() {
        val session = scannerViewModel.attendanceSession ?: return
        binding.containerSummary.removeAllViews()
        
        // Hide "Add Passenger" if no vehicle OR if it's a pedestrian flow
        val isVehicleArrival = session.arrivalType == "vehicle"
        binding.btnAddPassenger.visibility = if (session.vehicle == null || !isVehicleArrival) View.GONE else View.VISIBLE

        // 1. Display Driver/Primary Person
        session.driver?.let { driver ->
            val vehicleNote = if (!isVehicleArrival || session.vehicle == null) {
                ""
            } else if (session.isVehicleMatched) {
                " ... came with his own vehicle"
            } else {
                " ... came with ${session.vehicleOwnerName ?: "unknown"}'s vehicle"
            }
            addSummaryCard(driver, vehicleNote)
        }

        // 2. Display Passengers
        session.passengers.forEach { passenger ->
            val driverName = session.driver?.firstNames ?: "Driver"
            val note = if (isVehicleArrival) " ... $driverName's vehicle" else ""
            addSummaryCard(passenger, note)
        }
    }

    private fun addSummaryCard(staff: StaffRecord, note: String) {
        val card = MaterialCardView(requireContext()).apply {
            layoutParams = LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
            ).apply { setMargins(0, 0, 0, 16) }
            radius = 16f
            setCardBackgroundColor(requireContext().getColor(R.color.scrim))
            strokeWidth = 0
            elevation = 0f
        }

        val layout = LinearLayout(requireContext()).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(20, 20, 20, 20)
        }

        val text = "${staff.firstNames} ${staff.lastName}$note"
        val statusTv = TextView(requireContext()).apply {
            this.text = text
            setTextColor(requireContext().getColor(R.color.white))
            textSize = 16f
            setTypeface(null, android.graphics.Typeface.BOLD)
        }

        if (staff.missingStateWarning != null) {
            val warningTv = TextView(requireContext()).apply {
                this.text = if (staff.missingStateWarning == "Invalid Passcode") "INVALID PASSCODE" else "WARNING: Missing state log"
                setTextColor(requireContext().getColor(R.color.primary_red))
                textSize = 12f
                setPadding(0, 4, 0, 0)
            }
            layout.addView(warningTv)
        }

        layout.addView(statusTv)
        card.addView(layout)
        binding.containerSummary.addView(card)
    }

    private fun saveSession() {
        val session = scannerViewModel.attendanceSession ?: return
        lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.btnFinalize.isEnabled = false
            try {
                // 1. Save Driver
                session.driver?.let { recordAttendance(it, session.scanType) }
                
                // 2. Save Passengers
                session.passengers.forEach { recordAttendance(it, session.scanType) }
                
                Toast.makeText(requireContext(), "Attendance Saved Successfully!", Toast.LENGTH_LONG).show()
                scannerViewModel.attendanceSession = null
                findNavController().popBackStack(R.id.actionHubFragment, false)
                
            } catch (e: Exception) {
                val errorMsg = e.localizedMessage ?: e.message ?: "Unknown"
                if (errorMsg.contains("document_type")) {
                    Toast.makeText(requireContext(), "Finalize Error: Database column 'document_type' is missing. Please run the migration SQL.", Toast.LENGTH_LONG).show()
                } else {
                    Toast.makeText(requireContext(), "Finalize Error: $errorMsg", Toast.LENGTH_LONG).show()
                }
            } finally {
                binding.progress.visibility = View.GONE
                binding.btnFinalize.isEnabled = true
            }
        }
    }

    private suspend fun recordAttendance(staff: StaffRecord, type: String) {
        if (staff.missingStateWarning == "Invalid Passcode") return
        val session = scannerViewModel.attendanceSession

        // If there was a warning, record it first
        staff.missingStateWarning?.let { warning ->
            val warningLog = AccessLogInsert(
                profileId = staff.id,
                scanType = warning,
                reissueQr = session?.needsNewQrCode ?: false,
                documentType = session?.documentTypeUsed
            )
            SupabaseManager.client.postgrest["access_logs"].insert(warningLog)
        }
        
        // Record the actual scan
        val logEntry = AccessLogInsert(
            profileId = staff.id,
            scanType = type,
            reissueQr = session?.needsNewQrCode ?: false,
            documentType = session?.documentTypeUsed
        )
        SupabaseManager.client.postgrest["access_logs"].insert(logEntry)
    }

    override fun onDestroyView() { super.onDestroyView(); _binding = null }

    @Serializable data class AccessLogSummary(@SerialName("scan_type") val scanType: String)
}
