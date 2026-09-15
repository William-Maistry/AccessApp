package com.openscansa.app.viewmodel

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.openscansa.app.models.ScanResult
import com.openscansa.app.models.StaffProfile
import com.openscansa.app.models.IdData
import com.openscansa.app.models.LicenseData
import com.openscansa.app.models.VehicleData
import com.openscansa.app.models.AttendanceSession
import com.openscansa.app.scanner.BarcodeAnalyzer
import com.peachss.sadldecoder.utils.LicenseInfo

class ScannerViewModel : ViewModel() {
    private val analyzer = BarcodeAnalyzer()
    private val _latestScan = MutableLiveData<ScanResult?>()
    val latestScan: LiveData<ScanResult?> = _latestScan
    private var lastRawValue = ""
    private var lastScanMillis = 0L

    var pendingStaffProfile: StaffProfile? = null
    var pendingIdData: IdData? = null
    var pendingLicenseData: LicenseData? = null
    var pendingVehicleData: VehicleData? = null

    var currentScanType: String = "in"

    // Attendance Group Session
    var attendanceSession: AttendanceSession? = null
    var pendingArrivalType: String? = null
    var pendingParticipantType: String? = null

    fun onBarcodeDetected(rawValue: String, format: String, licenseImageBytes: ByteArray? = null, licenseInfo: LicenseInfo? = null) {
        val now = System.currentTimeMillis()
        if (rawValue == lastRawValue && now - lastScanMillis < DUPLICATE_WINDOW_MS) return
        lastRawValue = rawValue
        lastScanMillis = now
        _latestScan.value = analyzer.toResult(rawValue, format, licenseImageBytes, licenseInfo)
    }

    fun clearResult() {
        _latestScan.value = null
        lastRawValue = ""
        lastScanMillis = 0L
    }

    private companion object { const val DUPLICATE_WINDOW_MS = 1_500L }
}
