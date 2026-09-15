package com.openscansa.app.models

import com.peachss.sadldecoder.utils.LicenseInfo

data class ScanResult(
    val rawValue: String,
    val displayValue: String,
    val format: String,
    val licenseImageBytes: ByteArray? = null,
    val licenseInfo: LicenseInfo? = null,
    val scannedAtMillis: Long = System.currentTimeMillis()
)
