package com.openscansa.app

import android.app.Application
import com.openscansa.app.auth.SupabaseManager

class OpenScanApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        SupabaseManager.initialize(this)
    }
}
