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
import com.openscansa.app.databinding.FragmentOnboardingBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.gotrue.SessionStatus
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class OnboardingFragment : Fragment() {
    private var _binding: FragmentOnboardingBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentOnboardingBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnHome.setOnClickListener {
            viewLifecycleOwner.lifecycleScope.launch {
                try {
                    SupabaseManager.client.auth.signOut()
                } catch (_: Exception) {}
                
                val intent = android.content.Intent(requireContext(), com.openscansa.app.MainActivity::class.java)
                intent.flags = android.content.Intent.FLAG_ACTIVITY_NEW_TASK or android.content.Intent.FLAG_ACTIVITY_CLEAR_TASK
                startActivity(intent)
            }
        }

        viewLifecycleOwner.lifecycleScope.launch {
            SupabaseManager.client.auth.sessionStatus.collectLatest { status ->
                if (status is SessionStatus.Authenticated) {
                    checkExistingProfile(status.session.user?.id ?: "", status.session.user?.email ?: "")
                }
            }
        }

        binding.btnProceed.setOnClickListener {
            saveProfile()
        }
    }

    private fun checkExistingProfile(userId: String, email: String) {
        if (userId.isEmpty()) return
        
        lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            try {
                // 1. Check if name exists
                val profile = SupabaseManager.client.postgrest["users"]
                    .select {
                        filter {
                            eq("id", userId)
                        }
                    }.decodeSingleOrNull<UserProfile>()

                if (profile != null && !profile.full_name.isNullOrBlank()) {
                    // Name exists, navigate based on role
                    navigateToDashboard(email)
                } else {
                    binding.cardContainer.visibility = View.VISIBLE
                }
            } catch (e: Exception) {
                binding.cardContainer.visibility = View.VISIBLE
            } finally {
                binding.progress.visibility = View.GONE
            }
        }
    }

    private suspend fun navigateToDashboard(email: String) {
        try {
            val whitelist = SupabaseManager.client.postgrest["allowed_emails"]
                .select {
                    filter {
                        eq("email", email)
                    }
                }.decodeSingleOrNull<WhitelistEntry>()
            
            if (whitelist?.role == "admin") {
                findNavController().navigate(R.id.action_onboardingFragment_to_adminDashboardFragment)
            } else {
                findNavController().navigate(R.id.action_onboardingFragment_to_actionHubFragment)
            }
        } catch (e: Exception) {
            // Fallback to Action Hub if role check fails
            findNavController().navigate(R.id.action_onboardingFragment_to_actionHubFragment)
        }
    }

    private fun saveProfile() {
        val name = binding.editFullName.text.toString().trim()
        if (name.isEmpty()) {
            Toast.makeText(requireContext(), "Please enter your name", Toast.LENGTH_SHORT).show()
            return
        }

        val user = SupabaseManager.client.auth.currentUserOrNull() ?: return

        lifecycleScope.launch {
            binding.progress.visibility = View.VISIBLE
            binding.btnProceed.isEnabled = false
            try {
                val userProfile = UserProfile(id = user.id, full_name = name)
                SupabaseManager.client.postgrest["users"].upsert(userProfile)
                
                navigateToDashboard(user.email ?: "")
            } catch (e: Exception) {
                Toast.makeText(requireContext(), "Error saving name: ${e.message}", Toast.LENGTH_LONG).show()
            } finally {
                binding.progress.visibility = View.GONE
                binding.btnProceed.isEnabled = true
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class UserProfile(val id: String, val full_name: String?)

    @Serializable
    data class WhitelistEntry(val email: String, val role: String?)
}
