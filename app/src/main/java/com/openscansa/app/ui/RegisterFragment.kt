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
import com.openscansa.app.databinding.FragmentRegisterBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.gotrue.providers.builtin.Email
import io.github.jan.supabase.postgrest.postgrest
import kotlinx.coroutines.launch

class RegisterFragment : Fragment() {
    private var _binding: FragmentRegisterBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentRegisterBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnHome.setOnClickListener {
            findNavController().navigate(R.id.landingFragment)
        }

        binding.btnDoRegister.setOnClickListener {
            val emailText = binding.editEmail.text.toString().trim()
            val passwordText = binding.editPin.text.toString().trim()

            if (emailText.isEmpty() || passwordText.length < 6) {
                Toast.makeText(requireContext(), "Enter valid email and at least 6 characters", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            lifecycleScope.launch {
                binding.progress.visibility = View.VISIBLE
                binding.btnDoRegister.isEnabled = false
                try {
                    // 1. Check whitelist
                    val results = SupabaseManager.client.postgrest["allowed_emails"]
                        .select {
                            filter {
                                eq("email", emailText)
                            }
                        }.decodeList<WhitelistEntry>()

                    if (results.isEmpty()) {
                        Toast.makeText(requireContext(), "Email not authorized", Toast.LENGTH_LONG).show()
                        return@launch
                    }

                    // 2. Register
                    SupabaseManager.client.auth.signUpWith(Email, redirectUrl = "openscansa://dashboard") {
                        email = emailText
                        password = passwordText
                    }

                    Toast.makeText(requireContext(), "Registration successful! Check your email for confirmation.", Toast.LENGTH_LONG).show()
                    findNavController().popBackStack()

                } catch (e: Exception) {
                    Toast.makeText(requireContext(), "Error: ${e.message}", Toast.LENGTH_LONG).show()
                } finally {
                    binding.progress.visibility = View.GONE
                    binding.btnDoRegister.isEnabled = true
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    @kotlinx.serialization.Serializable
    data class WhitelistEntry(val email: String, val role: String? = "guard")
}
