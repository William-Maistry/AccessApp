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
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.databinding.FragmentVehicleEntryBinding
import com.openscansa.app.models.VehicleData
import com.openscansa.app.viewmodel.ScannerViewModel

class VehicleEntryFragment : Fragment() {
    private var _binding: FragmentVehicleEntryBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()

    private val companyVehicles = setOf(
        "DH10VWGP", "CX31WNGP", "CX31VVGP", "DC61FPGP", "KC95SJGP",
        "JD84LWGP", "JD84MNGP", "KK91JCGP", "KC57PRGP", "KC57PNGP",
        "KH33SYGP", "KV45LSGP", "KV45LJGP", "LG82TBGP", "LJ88WMGP",
        "LGO5SYGP", "LGO5RZGP", "LX98MFGP", "LX98MLGP", "KW63LVGP",
        "KX51RGGP", "MS66JWGP", "MS66JFGP", "MS66HZGP", "MS66KKGP",
        "MS76HNGP", "MS76HXGP", "MS76DPGP", "MS76FPGP", "MS76GDGP",
        "MS76GRGP", "MS76GXGP", "MS76HFGP", "MS76FJGP", "MS76DYGP",
        "FB71PVGP", "DZ8OYPGP", "DZ63LZGP", "LY53CVGP", "LY53FBGP",
        "FN58WHGP", "FS02TJGP", "JP65BZGP", "JP64YKGP", "JP65BVGP",
        "JP64YVGP", "XVB239GP", "TTY396GP", "ZTD390GP", "BF06WBGP",
        "BD52WKGP", "HS47HZGP", "MS60NYGP", "MS60NTGP", "M560NDGP",
        "MS60NGGP", "MS74GLGP"
    )

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentVehicleEntryBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnBack.setOnClickListener {
            findNavController().popBackStack()
        }

        binding.btnScanDisc.setOnClickListener {
            val args = Bundle().apply {
                putBoolean("returnResult", true)
                putString("requestKey", "vehicle_disc_scan")
            }
            findNavController().navigate(R.id.scannerFragment, args)
        }

        binding.btnScanPlate.setOnClickListener {
            Toast.makeText(requireContext(), "Coming soon", Toast.LENGTH_SHORT).show()
        }

        binding.btnNext.setOnClickListener {
            findNavController().navigate(R.id.action_vehicleEntryFragment_to_vehicleActionFragment)
        }

        parentFragmentManager.setFragmentResultListener("vehicle_disc_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                parseAndDisplayDisc(barcode)
            }
        }
    }

    private fun parseAndDisplayDisc(result: String) {
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
        displayVehicle(vehicleData)
    }

    private fun isCompanyVehicle(data: VehicleData): Boolean {
        val reg1 = data.vehicleRegisterNumber?.replace(" ", "")?.uppercase()?.trim()
        val reg2 = data.licenceNumber?.replace(" ", "")?.uppercase()?.trim()
        val reg3 = data.registerNumber?.replace(" ", "")?.uppercase()?.trim()
        val raw = data.rawData?.replace(" ", "")?.uppercase()?.trim()

        return listOfNotNull(reg1, reg2, reg3).any { companyVehicles.contains(it) } ||
               companyVehicles.any { raw?.contains(it) == true }
    }

    private fun displayVehicle(data: VehicleData) {
        binding.containerButtons.visibility = View.GONE
        binding.scrollResults.visibility = View.VISIBLE
        binding.btnNext.visibility = View.VISIBLE

        val isCompany = isCompanyVehicle(data)
        if (isCompany) {
            binding.cardCompanyVehicle.visibility = View.VISIBLE
        } else {
            binding.cardCompanyVehicle.visibility = View.GONE
        }

        binding.containerResults.removeAllViews()

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
}
