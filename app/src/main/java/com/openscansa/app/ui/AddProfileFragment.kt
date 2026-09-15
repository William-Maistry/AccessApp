package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentAddProfileBinding
import com.openscansa.app.models.StaffProfile
import com.openscansa.app.viewmodel.ScannerViewModel
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class AddProfileFragment : Fragment() {
    private var _binding: FragmentAddProfileBinding? = null
    private val binding get() = _binding!!
    private val scannerViewModel: ScannerViewModel by activityViewModels()
    private var userRole: String? = null

    private val positions = listOf(
        "Casual", "Contractor", "Finance", "IT", "Linehaul", 
        "Local", "Office", "Operations", "Sales", "Security",
        "Temp", "Visitor", "Warehouse", "Workshop", "Yard", "Other"
    )

    private val genders = listOf("Male", "Female")

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentAddProfileBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        setupDropdowns()
        fetchUserRole()

        binding.btnHome.setOnClickListener {
            navigateHome()
        }

        binding.spinnerPosition.setOnItemClickListener { _, _, position, _ ->
            val selected = positions[position]
            binding.layoutOtherPosition.visibility = if (selected == "Other") View.VISIBLE else View.GONE
        }

        binding.btnNext.setOnClickListener {
            validateAndProceed()
        }
    }

    private fun setupDropdowns() {
        val positionAdapter = ArrayAdapter(requireContext(), android.R.layout.simple_dropdown_item_1line, positions)
        binding.spinnerPosition.setAdapter(positionAdapter)

        val genderAdapter = ArrayAdapter(requireContext(), android.R.layout.simple_dropdown_item_1line, genders)
        binding.spinnerGender.setAdapter(genderAdapter)
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

    private fun validateAndProceed() {
        val firstNames = binding.editFirstNames.text.toString().trim()
        val lastName = binding.editLastName.text.toString().trim()
        val gender = binding.spinnerGender.text.toString()
        val phone = binding.editPhone.text.toString().trim()
        val secondaryPhone = binding.editSecondaryPhone.text.toString().trim()
        val email = binding.editEmail.text.toString().trim()
        val selectedPosition = binding.spinnerPosition.text.toString()
        val otherPosition = binding.editOtherPosition.text.toString().trim()

        if (firstNames.isEmpty()) {
            binding.editFirstNames.error = "Required"
            return
        }
        if (lastName.isEmpty()) {
            binding.editLastName.error = "Required"
            return
        }
        if (gender.isEmpty()) {
            Toast.makeText(requireContext(), "Please select gender", Toast.LENGTH_SHORT).show()
            return
        }
        if (phone.isEmpty()) {
            binding.editPhone.error = "Required"
            return
        }
        
        if (email.isNotEmpty() && !android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            binding.editEmail.error = "Invalid email format"
            return
        }

        if (selectedPosition.isEmpty()) {
            Toast.makeText(requireContext(), "Please select position", Toast.LENGTH_SHORT).show()
            return
        }

        val finalPosition = if (selectedPosition == "Other") {
            if (otherPosition.isEmpty()) {
                binding.editOtherPosition.error = "Please specify position"
                return
            }
            otherPosition
        } else {
            selectedPosition
        }

        val profile = StaffProfile(
            firstNames = firstNames,
            lastName = lastName,
            idNumber = null,
            gender = gender,
            phoneNumber = phone,
            secondaryPhone = secondaryPhone.ifEmpty { null },
            email = email.ifEmpty { null },
            position = finalPosition
        )

        scannerViewModel.pendingStaffProfile = profile
        findNavController().navigate(R.id.fragmentScanDocsIntro)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
