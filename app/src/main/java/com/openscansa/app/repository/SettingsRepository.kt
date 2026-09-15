package com.openscansa.app.repository

import android.content.Context

class SettingsRepository(context: Context) {
    private val preferences = context.getSharedPreferences("open_scan_settings", Context.MODE_PRIVATE)
    var torchEnabled: Boolean
        get() = preferences.getBoolean(KEY_TORCH, false)
        set(value) = preferences.edit().putBoolean(KEY_TORCH, value).apply()

    private companion object { const val KEY_TORCH = "torch_enabled" }
}
