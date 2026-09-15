package com.openscansa.app.auth

import android.content.Context
import io.github.jan.supabase.SupabaseClient
import io.github.jan.supabase.createSupabaseClient
import io.github.jan.supabase.gotrue.Auth
import io.github.jan.supabase.gotrue.SessionManager
import io.github.jan.supabase.gotrue.user.UserSession
import io.github.jan.supabase.postgrest.Postgrest
import io.github.jan.supabase.storage.Storage
import kotlinx.serialization.encodeToString
import kotlinx.serialization.json.Json

object SupabaseManager {
    private const val SUPABASE_URL = "https://txbzkgcgasgqetujffja.supabase.co"
    private const val SUPABASE_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InR4YnprZ2NnYXNncWV0dWpmZmphIiwicm9sZSI6ImFub24iLCJpYXQiOjE3ODQ1MzU0ODEsImV4cCI6MjEwMDExMTQ4MX0.mM3ZxCagF88FgKeWLxqYiMiU-ONZ_jqc_tPkafQoNK8"

    @Volatile
    private var _client: SupabaseClient? = null
    
    val client: SupabaseClient
        get() = _client ?: throw IllegalStateException("SupabaseManager not initialized. Call initialize(context) first.")

    fun initialize(context: Context) {
        if (_client != null) return
        
        synchronized(this) {
            if (_client == null) {
                _client = createSupabaseClient(
                    supabaseUrl = SUPABASE_URL,
                    supabaseKey = SUPABASE_KEY
                ) {
                    install(Auth) {
                        sessionManager = SharedPreferencesSessionManager(context)
                        autoLoadFromStorage = true
                        scheme = "openscansa"
                    }
                    install(Postgrest)
                    install(Storage)
                }
            }
        }
    }
}

class SharedPreferencesSessionManager(context: Context) : SessionManager {
    private val sharedPref = context.applicationContext.getSharedPreferences("supabase_session", Context.MODE_PRIVATE)

    override suspend fun saveSession(session: UserSession) {
        sharedPref.edit().putString("session_data", Json.encodeToString(session)).apply()
    }

    override suspend fun loadSession(): UserSession? {
        val sessionData = sharedPref.getString("session_data", null) ?: return null
        return try {
            Json.decodeFromString<UserSession>(sessionData)
        } catch (e: Exception) {
            null
        }
    }

    override suspend fun deleteSession() {
        sharedPref.edit().remove("session_data").apply()
    }
}
