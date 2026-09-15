package com.openscansa.app.ui

import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.findNavController
import com.openscansa.app.MainActivity
import com.openscansa.app.R
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.FragmentAdminDashboardBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

class AdminDashboardFragment : Fragment() {
    private var _binding: FragmentAdminDashboardBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentAdminDashboardBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        fetchAdminName()

        binding.btnGoToHub.setOnClickListener {
            findNavController().navigate(R.id.action_adminDashboardFragment_to_actionHubFragment)
        }

        binding.btnSignOut.setOnClickListener {
            viewLifecycleOwner.lifecycleScope.launch {
                try {
                    SupabaseManager.client.auth.signOut()
                } catch (_: Exception) {}
                
                val intent = Intent(requireContext(), MainActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK
                startActivity(intent)
            }
        }
    }

    private fun fetchAdminName() {
        val user = SupabaseManager.client.auth.currentUserOrNull() ?: return
        viewLifecycleOwner.lifecycleScope.launch {
            try {
                val profile = SupabaseManager.client.postgrest["users"]
                    .select {
                        filter {
                            eq("id", user.id)
                        }
                    }.decodeSingleOrNull<UserProfile>()
                
                if (profile != null && !profile.full_name.isNullOrBlank()) {
                    binding.greetingText.text = "Hello, ${profile.full_name}"
                }
            } catch (_: Exception) {}
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @Serializable
    data class UserProfile(val id: String, val full_name: String?)
}
