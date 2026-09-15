package com.openscansa.app.scanner

import com.openscansa.app.models.ScanResult
import com.openscansa.app.parser.BarcodeParser
import com.peachss.sadldecoder.utils.LicenseInfo

class BarcodeAnalyzer {
    fun toResult(rawValue: String, format: String, licenseImageBytes: ByteArray? = null, licenseInfo: LicenseInfo? = null): ScanResult =
        BarcodeParser.parse(rawValue, format, licenseImageBytes, licenseInfo)
}
