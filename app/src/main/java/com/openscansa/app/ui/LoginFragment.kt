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
import com.openscansa.app.databinding.FragmentLoginBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.gotrue.providers.builtin.Email
import kotlinx.coroutines.launch

class LoginFragment : Fragment() {
    private var _binding: FragmentLoginBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentLoginBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnHome.setOnClickListener {
            findNavController().navigate(R.id.landingFragment)
        }

        binding.btnDoLogin.setOnClickListener {
            val emailText = binding.editEmail.text.toString().trim()
            val passwordText = binding.editPin.text.toString().trim()

            if (emailText.isEmpty() || passwordText.isEmpty()) {
                Toast.makeText(requireContext(), "Enter email and password", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            lifecycleScope.launch {
                binding.progress.visibility = View.VISIBLE
                binding.btnDoLogin.isEnabled = false
                try {
                    SupabaseManager.client.auth.signInWith(Email) {
                        email = emailText
                        password = passwordText
                    }
                    findNavController().navigate(R.id.onboardingFragment)
                } catch (e: Exception) {
                    Toast.makeText(requireContext(), "Login failed: ${e.message}", Toast.LENGTH_LONG).show()
                } finally {
                    binding.progress.visibility = View.GONE
                    binding.btnDoLogin.isEnabled = true
                }
            }
        }

        binding.btnForgotPassword.setOnClickListener {
            findNavController().navigate(R.id.action_loginFragment_to_forgotPasswordFragment)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
