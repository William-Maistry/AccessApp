package com.openscansa.app.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.openscansa.app.R
import com.openscansa.app.databinding.FragmentLandingBinding

class LandingFragment : Fragment() {
    private var _binding: FragmentLandingBinding? = null
    private val binding get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentLandingBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        // Session check is now handled in MainActivity to prevent crashes/flicker

        binding.btnRegister.setOnClickListener {
            findNavController().navigate(R.id.action_landingFragment_to_registerFragment)
        }

        binding.btnLogin.setOnClickListener {
            findNavController().navigate(R.id.action_landingFragment_to_loginFragment)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
