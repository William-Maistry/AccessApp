package com.openscansa.app.parser

import com.openscansa.app.models.ScanResult
import com.peachss.sadldecoder.utils.LicenseInfo
import java.text.SimpleDateFormat
import java.util.Locale

object BarcodeParser {
    fun parse(rawValue: String, format: String, licenseImageBytes: ByteArray? = null, licenseInfo: LicenseInfo? = null): ScanResult {
        val trimmed = rawValue.trim()

        val display = when {
            licenseInfo != null -> formatLicenseInfo(licenseInfo)
            trimmed == "ENCRYPTED_SA_DL_DETECTED" -> "South African Driver's License detected, but decryption failed.\n\nThis usually happens if the license uses an unsupported encryption version or if the data is corrupted."
            trimmed.isEmpty() -> trimmed
            trimmed.contains('\n') || trimmed.contains('\r') -> trimmed
                .replace("\r\n", "\n")
                .replace("\r", "\n")
                .replace(Regex("\\n{3,}"), "\n\n")
            else -> trimmed.replace(Regex("\\s+"), " ")
        }
        
        return ScanResult(
            rawValue = if (trimmed == "ENCRYPTED_SA_DL_DETECTED") "" else trimmed,
            displayValue = display,
            format = format,
            licenseImageBytes = licenseImageBytes,
            licenseInfo = licenseInfo
        )
    }

    private fun formatLicenseInfo(info: LicenseInfo): String {
        val df = SimpleDateFormat("yyyy-MM-dd", Locale.getDefault())
        val now = System.currentTimeMillis()
        val isLicensed = info.licenseValidTo?.let { it.time > now } ?: false
        
        val vehicleCodes = listOfNotNull(info.licenseCode1, info.licenseCode2, info.licenseCode3, info.licenseCode4)
            .filter { it.isNotBlank() }
            .joinToString(", ")

        return buildString {
            append("SA DRIVER'S LICENSE\n")
            append("--------------------\n")
            append("Vehicle codes: $vehicleCodes\n")
            append("Surname: ${info.surname}\n")
            append("Initials: ${info.initials}\n")
            append("ID Country of Issue: ${info.idCountryOfIssue}\n")
            append("License Country of Issue: ${info.licenseCountryOfIssue}\n")
            append("Vehicle Restriction: ${info.vehicleRestriction1 ?: "0"}\n")
            append("ID Number: ${info.idNumber}\n")
            append("License Number: ${info.licenseNumber}\n")
            append("ID number type: ${info.idType}\n")
            append("License code issue date: ${info.licenseIssueDate1?.let { df.format(it) } ?: "N/A"}\n")
            append("Driver restriction codes: ${info.driverRestriction ?: "00"}\n")
            append("License issue number: ${info.licenseIssueNo}\n")
            append("Birthdate: ${info.birthDate?.let { df.format(it) } ?: "N/A"}\n")
            append("License Valid From: ${info.licenseValidFrom?.let { df.format(it) } ?: "N/A"}\n")
            append("License Valid To: ${info.licenseValidTo?.let { df.format(it) } ?: "N/A"}\n")
            append("Gender: ${info.gender}\n")
            append("Licensed: ${if (isLicensed) "Yes" else "No"}")
        }
    }
}
