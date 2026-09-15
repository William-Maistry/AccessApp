package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentScanDocumentsIntroBinding
import com.openscansa.app.models.IdData
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class ScanDocumentsIntroFragment : Fragment() {
    private var _binding: FragmentScanDocumentsIntroBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private var userRole: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentScanDocumentsIntroBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        fetchUserRole()

        // Listen for scan result
        parentFragmentManager.setFragmentResultListener("scan_request", viewLifecycleOwner) { _, bundle ->
            val result = bundle.getString("barcode")
            if (result != null) {
                parseAndDisplayResult(result)
            }
        }

        binding.btnHome.setOnClickListener {
            if (userRole == "admin") {
                findNavController().navigate(R.id.adminDashboardFragment)
            } else {
                findNavController().navigate(R.id.actionHubFragment)
            }
        }

        binding.btnProceed.setOnClickListener {
            val args = Bundle().apply {
                putBoolean("returnResult", true)
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnNext.setOnClickListener {
            findNavController().navigate(R.id.scanLicenseIntroFragment)
        }
    }

    private fun parseAndDisplayResult(result: String) {
        val idData = if (result.contains("|")) {
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

        scannerViewModel.pendingIdData = idData
        displayParsedFields(idData)
    }

    private fun displayParsedFields(idData: IdData) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE
        binding.btnNext.visibility = View.VISIBLE
        binding.btnProceed.text = "Rescan ID"

        if (idData.isParsed) {
            addField("Surname", idData.surname)
            addField("First Name/s", idData.firstNames)
            addField("Sex", idData.sex)
            addField("Nationality", idData.nationality)
            addField("ID Number", idData.idNumber)
            addField("Date of Birth", idData.dob)
            addField("Citizenship Country", idData.citizenshipCountry)
            addField("Citizen Type", idData.citizenType)
            addField("Card Issue Date", idData.cardIssueDate)
            addField("Issuing Office", idData.issuingOfficeCode)
            addField("Record Number", idData.internalRecordNumber)
        } else {
            addField("ID Number", idData.idNumber)
        }
    }

    private fun addField(label: String, value: String?) {
        if (value.isNullOrBlank()) return
        
        val row = LinearLayout(requireContext()).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(0, 0, 0, 16)
        }

        val labelTv = TextView(requireContext()).apply {
            text = label
            setTextColor(requireContext().getColor(R.color.grey))
            textSize = 12f
        }

        val valueTv = TextView(requireContext()).apply {
            text = value
            setTextColor(requireContext().getColor(R.color.white))
            textSize = 18f
            setPadding(0, 4, 0, 0)
        }

        row.addView(labelTv)
        row.addView(valueTv)
        binding.containerResults.addView(row)
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

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
