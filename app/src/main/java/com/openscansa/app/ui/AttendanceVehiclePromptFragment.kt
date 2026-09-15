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
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentAttendanceVehiclePromptBinding
import com.openscansa.app.models.VehicleData
import com.openscansa.app.utils.DateUtils
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

class AttendanceVehiclePromptFragment : Fragment() {
    private var _binding: FragmentAttendanceVehiclePromptBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentAttendanceVehiclePromptBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnHome.setOnClickListener {
            findNavController().popBackStack(R.id.actionHubFragment, false)
        }

        binding.btnYes.setOnClickListener {
            // Open camera for vehicle scan
            val args = Bundle().apply {
                putBoolean("returnResult", true)
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnNo.setOnClickListener {
            // User confirmed no vehicle, clear any potentially scanned data
            scannerViewModel.attendanceSession?.let {
                it.vehicle = null
                it.vehicleOwnerName = null
                it.isVehicleMatched = false
            }
            findNavController().navigate(R.id.attendanceSummaryFragment)
        }

        binding.btnNext.setOnClickListener {
            findNavController().navigate(R.id.attendanceSummaryFragment)
        }

        // Listen for vehicle scan result
        parentFragmentManager.setFragmentResultListener("scan_request", viewLifecycleOwner) { _, bundle ->
            val result = bundle.getString("barcode")
            if (result != null) {
                processVehicleScan(result)
            }
        }
    }

    private fun processVehicleScan(barcode: String) {
        val session = scannerViewModel.attendanceSession ?: return
        
        val vehicleData = if (barcode.contains("%")) {
            val cleaned = barcode.trim('%')
            val parts = cleaned.split("%")
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
                rawData = barcode
            )
        } else {
            VehicleData(rawData = barcode)
        }

        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.cardError.visibility = View.GONE
            binding.cardWarning.visibility = View.GONE
            try {
                // 1. Expiry Check
                val today = DateUtils.getNetworkDate()
                if (DateUtils.isExpired(vehicleData.expiryDate, today)) {
                    binding.tvErrorMessage.text = "EXPIRED: The Vehicle Registration has expired (${vehicleData.expiryDate})."
                    binding.cardError.visibility = View.VISIBLE
                    binding.btnNext.visibility = View.GONE
                } else {
                    val daysUntil = DateUtils.getDaysUntilExpiry(vehicleData.expiryDate, today)
                    if (daysUntil <= 30) {
                        binding.tvWarningMessage.text = "WARNING: Vehicle Registration expires in $daysUntil days."
                        binding.cardWarning.visibility = View.VISIBLE
                    }
                    binding.btnNext.visibility = View.VISIBLE
                }

                // 2. Ownership Check
                session.vehicle = vehicleData
                val driverLicense = session.driver?.licenceNumber
                if (vehicleData.licenceNumber != null && vehicleData.licenceNumber != driverLicense) {
                    // Search whose vehicle it is
                    val owner = SupabaseManager.client.postgrest["profiles"]
                        .select {
                            filter {
                                eq("licence_number", vehicleData.licenceNumber)
                            }
                        }.decodeSingleOrNull<StaffSummary>()
                    
                    session.vehicleOwnerName = owner?.firstNames
                    session.isVehicleMatched = false
                } else {
                    session.isVehicleMatched = true
                    session.vehicleOwnerName = null
                }

                displayParsedFields(vehicleData)
                binding.btnYes.text = "Rescan Vehicle"
                binding.tvQuestion.visibility = View.GONE

            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Verification Error: ${e.message}", Toast.LENGTH_LONG).show()
                displayParsedFields(vehicleData)
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun displayParsedFields(data: VehicleData) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE

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

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class StaffSummary(
        @SerialName("first_names") val firstNames: String
    )
}
