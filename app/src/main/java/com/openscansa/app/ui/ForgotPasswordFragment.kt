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
import com.openscansa.app.databinding.FragmentForgotPasswordBinding
import io.github.jan.supabase.gotrue.auth
import kotlinx.coroutines.launch

class ForgotPasswordFragment : Fragment() {
    private var _binding: FragmentForgotPasswordBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentForgotPasswordBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        binding.btnHome.setOnClickListener {
            findNavController().navigate(R.id.landingFragment)
        }

        binding.btnSendReset.setOnClickListener {
            val emailText = binding.editEmail.text.toString().trim()
            if (emailText.isEmpty()) {
                Toast.makeText(requireContext(), "Please enter your email", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            lifecycleScope.launch {
                binding.progress.visibility = View.VISIBLE
                binding.btnSendReset.isEnabled = false
                try {
                    SupabaseManager.client.auth.resetPasswordForEmail(
                        email = emailText,
                        redirectUrl = "openscansa://reset-password"
                    )
                    Toast.makeText(requireContext(), "Reset link sent to your email", Toast.LENGTH_LONG).show()
                } catch (e: Exception) {
                    Toast.makeText(requireContext(), "Error: ${e.message}", Toast.LENGTH_LONG).show()
                } finally {
                    binding.progress.visibility = View.GONE
                    binding.btnSendReset.isEnabled = true
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
