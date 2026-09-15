package com.openscansa.app.ui

import android.graphics.Bitmap
import android.graphics.BitmapFactory
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
import com.openscansa.app.databinding.FragmentScanLicenseIntroBinding
import com.openscansa.app.models.LicenseData
import com.openscansa.app.utils.DateUtils
import com.openscansa.app.viewmodel.ScannerViewModel
import com.peachss.sadldecoder.utils.LicenseInfo
import com.peachss.sadldecoder.utils.SADLUtils
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable
import java.text.SimpleDateFormat
import java.util.Locale

class ScanLicenseIntroFragment : Fragment() {
    private var _binding: FragmentScanLicenseIntroBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private var userRole: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentScanLicenseIntroBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        fetchUserRole()

        // Listen for scan result
        parentFragmentManager.setFragmentResultListener("scan_request", viewLifecycleOwner) { _, bundle ->
            val hasLicenseInfo = bundle.getBoolean("hasLicenseInfo")
            val barcode = bundle.getString("barcode")
            
            if (hasLicenseInfo) {
                val data = scannerViewModel.pendingLicenseData
                if (data != null) {
                    validateAndDisplayLicense(data)
                }
            } else if (barcode != null) {
                displayRawBarcode(barcode)
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
            findNavController().navigate(R.id.scanVehicleRegistrationIntroFragment)
        }

        binding.btnNext.setOnClickListener {
            findNavController().navigate(R.id.scanVehicleRegistrationIntroFragment)
        }
    }

    private fun validateAndDisplayLicense(data: LicenseData) {
        viewLifecycleOwner.lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.cardError.visibility = View.GONE
            binding.cardWarning.visibility = View.GONE
            try {
                val today = DateUtils.getNetworkDate()
                if (DateUtils.isExpired(data.validTo, today)) {
                    binding.tvErrorMessage.text = "EXPIRED: The Driver's License has expired (${data.validTo})."
                    binding.cardError.visibility = View.VISIBLE
                    binding.btnNext.visibility = View.GONE
                } else {
                    val daysUntil = DateUtils.getDaysUntilExpiry(data.validTo, today)
                    if (daysUntil <= 30) {
                        binding.tvWarningMessage.text = "WARNING: The Driver's License expires in $daysUntil days (${data.validTo})."
                        binding.cardWarning.visibility = View.VISIBLE
                    }
                    binding.btnNext.visibility = View.VISIBLE
                }
                displayLicenseData(data)
            } catch (_: Exception) {
                displayLicenseData(data)
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private fun displayLicenseData(data: LicenseData) {
        // Clear previous dynamic fields (keep the image view)
        val container = binding.containerResults
        val viewsToRemove = mutableListOf<View>()
        for (i in 0 until container.childCount) {
            val child = container.getChildAt(i)
            if (child.id != R.id.license_image) {
                viewsToRemove.add(child)
            }
        }
        viewsToRemove.forEach { container.removeView(it) }

        binding.scrollResults.visibility = View.VISIBLE
        binding.btnScan.text = "Rescan License"

        // Handle image
        if (data.photoBytes != null) {
            val bitmap = decodeLicenseImageBitmap(data.photoBytes)
            if (bitmap != null) {
                binding.licenseImage.setImageBitmap(bitmap)
                binding.licenseImage.visibility = View.VISIBLE
            } else {
                binding.licenseImage.visibility = View.GONE
            }
        } else {
            binding.licenseImage.visibility = View.GONE
        }

        // Basic Info
        addField("Surname", data.surname)
        addField("Initials", data.initials)
        addField("ID Number", data.idNumber)
        addField("Gender", data.gender)
        addField("Date of Birth", data.birthDate)
        
        // License Details
        addField("License Number", data.licenseNumber)
        addField("Issue Number", data.issueNumber)
        addField("Country of Issue", data.countryOfIssue)
        
        // Dates
        addField("Valid From", data.validFrom)
        addField("Expiry Date", data.validTo)
        
        // Codes & Restrictions
        addField("Vehicle Codes", data.vehicleCodes)
        addField("Vehicle Restrictions", data.vehicleRestrictions)
        addField("Driver Restriction", data.driverRestriction)
        
        // PDP
        if (!data.pdpCode.isNullOrBlank()) {
            addField("PDP Code", data.pdpCode)
            addField("PDP Expiry", data.pdpExpiry)
        }
    }

    private fun decodeLicenseImageBitmap(imageBytes: ByteArray): Bitmap? {
        if (imageBytes.isEmpty()) return null
        val decoded = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size)
        if (decoded != null) return decoded
        return try {
            SADLUtils.getBitmapFromImageBytes(imageBytes)
        } catch (_: Exception) {
            null
        }
    }

    private fun displayRawBarcode(barcode: String) {
        binding.containerResults.removeAllViews()
        binding.scrollResults.visibility = View.VISIBLE
        binding.btnNext.visibility = View.VISIBLE
        binding.btnScan.text = "Rescan License"
        binding.licenseImage.visibility = View.GONE

        addField("Barcode Data", barcode)
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
