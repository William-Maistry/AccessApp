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
import com.openscansa.app.databinding.FragmentScanVehicleRegistrationIntroBinding
import com.openscansa.app.models.VehicleData
import com.openscansa.app.utils.DateUtils
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class ScanVehicleRegistrationIntroFragment : Fragment() {
    private var _binding: FragmentScanVehicleRegistrationIntroBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private var userRole: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentScanVehicleRegistrationIntroBinding.inflate(inflater, container, false)
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
            navigateHome()
        }

        binding.btnScan.setOnClickListener {
            val args = Bundle().apply {
                putBoolean("returnResult", true)
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnSkip.setOnClickListener {
            findNavController().navigate(R.id.finalizationFragment)
        }

        binding.btnNext.setOnClickListener {
            findNavController().navigate(R.id.finalizationFragment)
        }
    }

    private fun parseAndDisplayResult(result: String) {
        val vehicleData = if (result.contains("%")) {
            val cleanedResult = result.trim('%')
            val parts = cleanedResult.split("%")
            
            VehicleData(
                controlCode = parts.getOrNull(0),
                licenceCode = parts.getOrNull(1),
                registerNumber = parts.getOrNull(2),
                sequenceNumber = parts.getOrNull(3),
                controlNumber = parts.getOrNull(4),
                licenceNumber = parts.getOrNull(5),
                vehicleRegisterNumber = parts.getOrNull(6),
                vehicleType = parts.getOrNull(7),
                make = parts.getOrNull(8),
                model = parts.getOrNull(9),
                colour = parts.getOrNull(10),
                vin = parts.getOrNull(11),
                engineNumber = parts.getOrNull(12),
                expiryDate = parts.getOrNull(13),
                rawData = result
            )
        } else {
            VehicleData(rawData = result)
        }

        scannerViewModel.pendingVehicleData = vehicleData
        validateAndDisplayVehicle(vehicleData)
    }

    private fun validateAndDisplayVehicle(data: VehicleData) {
        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.cardError.visibility = View.GONE
            binding.cardWarning.visibility = View.GONE
            try {
                val today = DateUtils.getNetworkDate()
                if (DateUtils.isExpired(data.expiryDate, today)) {
                    binding.tvErrorMessage.text = "EXPIRED: The Vehicle Registration has expired (${data.expiryDate})."
                    binding.cardError.visibility = View.VISIBLE
                    binding.btnNext.visibility = View.GONE
                } else {
                    val daysUntil = DateUtils.getDaysUntilExpiry(data.expiryDate, today)
                    if (daysUntil <= 30) {
                        binding.tvWarningMessage.text = "WARNING: The Vehicle Registration expires in $daysUntil days (${data.expiryDate})."
                        binding.cardWarning.visibility = View.VISIBLE
                    }
                    binding.btnNext.visibility = View.VISIBLE
                }
                displayParsedFields(data)
            } catch (_: Exception) {
                displayParsedFields(data)
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun displayParsedFields(data: VehicleData) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE
        binding.btnScan.text = "Rescan Vehicle"

        if (data.controlCode != null) {
            addField("Control Code", data.controlCode)
            addField("Licence Code", data.licenceCode)
            addField("Register Number", data.registerNumber)
            addField("Sequence Number", data.sequenceNumber)
            addField("Control Number", data.controlNumber)
            addField("Licence Number", data.licenceNumber)
            addField("Vehicle Register Number", data.vehicleRegisterNumber)
            addField("Vehicle Type", data.vehicleType)
            addField("Make", data.make)
            addField("Model", data.model)
            addField("Colour", data.colour)
            addField("VIN", data.vin)
            addField("Engine Number", data.engineNumber)
            addField("Expiry Date", data.expiryDate)
        } else {
            addField("Raw Data", data.rawData)
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
