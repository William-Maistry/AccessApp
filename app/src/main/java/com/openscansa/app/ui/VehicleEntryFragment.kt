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
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentVehicleEntryBinding
import com.openscansa.app.models.VehicleData
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch

class VehicleEntryFragment : Fragment() {
    private var _binding: FragmentVehicleEntryBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()

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
            findNavController().navigate(R.id.action_vehicleEntryFragment_to_plateScannerFragment)
        }

        parentFragmentManager.setFragmentResultListener("vehicle_disc_scan", viewLifecycleOwner) { _, bundle ->
            val barcode = bundle.getString("barcode")
            if (barcode != null) {
                parseAndVerifyDisc(barcode)
            }
        }
    }

    private fun parseAndVerifyDisc(barcode: String) {
        val vehicleData = if (barcode.contains("%")) {
            val cleanedResult = barcode.trim('%')
            val parts = cleanedResult.split("%")
            VehicleData(
                licenceNumber = parts.getOrNull(5),
                vehicleRegisterNumber = parts.getOrNull(6),
                rawData = barcode
            )
        } else {
            VehicleData(rawData = barcode)
        }

        val regNo = vehicleData.vehicleRegisterNumber ?: vehicleData.licenceNumber ?: ""
        if (regNo.isEmpty()) {
            Toast.makeText(requireContext(), "Invalid disc data", Toast.LENGTH_LONG).show()
            return
        }

        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                val match = SupabaseManager.client.postgrest["vehicle_registrations"]
                    .select {
                        filter {
                            or {
                                eq("vehicle_register_number", regNo)
                                eq("licence_number", regNo)
                            }
                        }
                    }.decodeSingleOrNull<VehicleData>()

                if (match != null) {
                    scannerViewModel.pendingVehicleData = match
                    findNavController().navigate(R.id.vehicleActionFragment)
                } else {
                    Toast.makeText(requireContext(), "Vehicle not registered: $regNo", Toast.LENGTH_LONG).show()
                }
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Database error: ${e.message}", Toast.LENGTH_LONG).show()
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
