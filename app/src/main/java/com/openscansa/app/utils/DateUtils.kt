package com.openscansa.app.utils

import com.openscansa.app.auth.SupabaseManager
import io.github.jan.supabase.postgrest.postgrest
import io.github.jan.supabase.postgrest.rpc
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import android.util.Log
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale
import java.util.TimeZone
import java.util.concurrent.TimeUnit

object DateUtils {
    private val dateFormat = SimpleDateFormat("yyyy-MM-dd", Locale.getDefault())

    suspend fun getNetworkDate(): Date = withContext(Dispatchers.IO) {
        try {
            val response = SupabaseManager.client.postgrest.rpc("get_now")
            val isoString = response.data.replace("\"", "")
            val parser = SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss", Locale.getDefault())
            parser.timeZone = TimeZone.getTimeZone("UTC")
            parser.parse(isoString) ?: Date()
        } catch (e: Exception) {
            Log.w("OpenScanSA", "Network time fetch failed: ${e.message}")
            Date()
        }
    }

    fun isExpired(dateString: String?, today: Date): Boolean {
        if (dateString.isNullOrBlank()) return false
        return try {
            val expiryDate = dateFormat.parse(dateString)
            expiryDate != null && expiryDate.before(today)
        } catch (e: Exception) {
            false
        }
    }

    fun getDaysUntilExpiry(dateString: String?, today: Date): Long {
        if (dateString.isNullOrBlank()) return Long.MAX_VALUE
        return try {
            val expiryDate = dateFormat.parse(dateString) ?: return Long.MAX_VALUE
            val diff = expiryDate.time - today.time
            TimeUnit.MILLISECONDS.toDays(diff)
        } catch (e: Exception) {
            Long.MAX_VALUE
        }
    }
}
