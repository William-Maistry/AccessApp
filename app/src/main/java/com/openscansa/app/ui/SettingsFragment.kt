package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.fragment.app.activityViewModels
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentSettingsBinding
import com.openscansa.app.viewmodel.SettingsViewModel
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class SettingsFragment : Fragment() {
    private var _binding: FragmentSettingsBinding? = null
    private val binding get() = requireNotNull(_binding)
    private val viewModel: SettingsViewModel by activityViewModels()
    private var userRole: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentSettingsBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        
        fetchUserRole()

        binding.btnHome.setOnClickListener {
            navigateHome()
        }

        viewModel.torchEnabled.observe(viewLifecycleOwner) { enabled ->
            if (binding.torchSwitch.isChecked != enabled) binding.torchSwitch.isChecked = enabled
        }
        binding.torchSwitch.setOnCheckedChangeListener { _, checked -> viewModel.setTorchEnabled(checked) }
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

    override fun onDestroyView() { _binding = null; super.onDestroyView() }

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
