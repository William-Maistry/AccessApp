package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentResetPasswordBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.gotrue.SessionStatus
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class ResetPasswordFragment : Fragment() {
    private var _binding: FragmentResetPasswordBinding? = null
    private val binding get() = _binding!!
    private var isSessionReady = false
    private var userRole: String? = null

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentResetPasswordBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // Initially disable UI until session is verified
        updateUiState(false)

        viewLifecycleOwner.lifecycleScope.launch {
            SupabaseManager.client.auth.sessionStatus.collectLatest { status ->
                if (status is SessionStatus.Authenticated) {
                    isSessionReady = true
                    fetchUserRole(status.session.user?.email ?: "")
                    updateUiState(true)
                } else {
                    isSessionReady = false
                    updateUiState(false)
                }
            }
        }

        binding.btnHome.setOnClickListener {
            if (userRole == "admin") {
                findNavController().navigate(R.id.adminDashboardFragment)
            } else if (userRole == "guard") {
                findNavController().navigate(R.id.actionHubFragment)
            } else {
                findNavController().navigate(R.id.landingFragment)
            }
        }

        binding.btnUpdatePassword.setOnClickListener {
            if (!isSessionReady) {
                Toast.makeText(requireContext(), "Verifying security token...", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }
            saveNewPassword()
        }
    }

    private fun updateUiState(ready: Boolean) {
        binding.btnUpdatePassword.isEnabled = ready
        binding.progress.visibility = if (ready) View.GONE else View.VISIBLE
        if (!ready) {
            binding.title.text = "Verifying Token..."
        } else {
            binding.title.text = "Set New Password"
        }
    }

    private fun saveNewPassword() {
        val newPassword = binding.editNewPassword.text.toString().trim()
        if (newPassword.length < 6) {
            Toast.makeText(requireContext(), "Password must be at least 6 characters", Toast.LENGTH_SHORT).show()
            return
        }

        lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.btnUpdatePassword.isEnabled = false
            try {
                SupabaseManager.client.auth.updateUser {
                    password = newPassword
                }
                Toast.makeText(requireContext(), "Password updated successfully!", Toast.LENGTH_LONG).show()
                findNavController().navigate(R.id.scannerFragment)
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Error: ${e.message}", Toast.LENGTH_LONG).show()
            } finally {
                binding.progress.visibility = View.GONE
                binding.btnUpdatePassword.isEnabled = true
            }
        }
    }

    private fun fetchUserRole(email: String) {
        if (email.isEmpty()) return
        viewLifecycleOwner.lifecycleScope.launch {
            try {
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
