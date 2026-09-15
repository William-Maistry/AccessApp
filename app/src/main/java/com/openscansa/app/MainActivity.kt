package com.openscansa.app

import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.lifecycle.lifecycleScope
import androidx.navigation.fragment.NavHostFragment
import androidx.navigation.ui.setupWithNavController
import com.openscansa.app.auth.SupabaseManager
import com.openscansa.app.databinding.ActivityMainBinding
import io.github.jan.supabase.gotrue.auth
import io.github.jan.supabase.gotrue.handleDeeplinks
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {
    private var isSessionCheckComplete = false

    override fun onCreate(savedInstanceState: Bundle?) {
        val splashScreen = installSplashScreen()
        super.onCreate(savedInstanceState)
        
        // Keep splash screen on until session check is done
        splashScreen.setKeepOnScreenCondition { !isSessionCheckComplete }

        val binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        
        val navHostFragment = supportFragmentManager.findFragmentById(R.id.nav_host) as NavHostFragment
        val navController = navHostFragment.navController
        
        // Setup Bottom Navigation
        binding.bottomNavigation.setupWithNavController(navController)

        // Handle incoming Supabase deep links
        handleSupabaseIntent(intent, navController)

        // Show/Hide Bottom Navigation based on destination
        navController.addOnDestinationChangedListener { _, destination, _ ->
            when (destination.id) {
                R.id.landingFragment, R.id.loginFragment, R.id.registerFragment, 
                R.id.forgotPasswordFragment, R.id.resetPasswordFragment, 
                R.id.onboardingFragment, R.id.adminDashboardFragment, R.id.actionHubFragment -> {
                    binding.bottomNavigation.visibility = View.GONE
                    window.statusBarColor = getColor(R.color.black)
                    window.navigationBarColor = getColor(R.color.black)
                }
                else -> {
                    binding.bottomNavigation.visibility = View.VISIBLE
                    window.statusBarColor = getColor(R.color.primary_red)
                }
            }
        }

        // Initial navigation logic for manual starts (no deep link)
        if (intent?.data == null) {
            lifecycleScope.launch {
                try {
                    val session = try {
                        SupabaseManager.client.auth.currentSessionOrNull()
                    } catch (e: Exception) {
                        null
                    }
                    
                    if (session != null) {
                        val navGraph = navController.navInflater.inflate(R.navigation.nav_graph)
                        navGraph.setStartDestination(R.id.onboardingFragment)
                        navController.setGraph(navGraph, null)
                    } else {
                        if (navController.currentDestination == null) {
                             navController.setGraph(R.navigation.nav_graph)
                        }
                    }
                } finally {
                    isSessionCheckComplete = true
                }
            }
        }
    }

    override fun onNewIntent(intent: Intent) {
        super.onNewIntent(intent)
        setIntent(intent)
        val navHostFragment = supportFragmentManager.findFragmentById(R.id.nav_host) as NavHostFragment
        handleSupabaseIntent(intent, navHostFragment.navController)
    }

    private fun handleSupabaseIntent(intent: Intent?, navController: androidx.navigation.NavController) {
        val uriString = intent?.data?.toString() ?: return

        // 1. Immediate Navigation based on URI content
        // Prevent double navigation if we're already where we need to be
        val currentDestId = navController.currentDestination?.id
        
        if (uriString.contains("reset-password")) {
            if (currentDestId != R.id.resetPasswordFragment) {
                navController.navigate(R.id.resetPasswordFragment)
            }
        } else if (uriString.contains("dashboard")) {
            if (currentDestId != R.id.onboardingFragment) {
                navController.navigate(R.id.onboardingFragment)
            }
        }

        // 2. Let Supabase process the link in the background
        SupabaseManager.client.handleDeeplinks(intent) { _ ->
            isSessionCheckComplete = true
        }
        
        // Safety: ensure splash screen eventually disappears
        lifecycleScope.launch {
            kotlinx.coroutines.delay(2000)
            isSessionCheckComplete = true
        }
    }
}
