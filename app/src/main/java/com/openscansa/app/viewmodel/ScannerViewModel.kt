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
import com.openscansa.app.models.StaffRecord
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

    fun addParticipant(record: StaffRecord) {
        val session = attendanceSession ?: return
        
        if (session.driver == null) {
            session.driver = record
            // Vehicle Ownership Check
            session.vehicle?.let { v ->
                if (v.licenceNumber == record.licenceNumber) {
                    session.isVehicleMatched = true
                    session.vehicleOwnerName = "${record.firstNames} ${record.lastName}"
                } else {
                    session.isVehicleMatched = false
                    session.vehicleOwnerName = "Registered Owner"
                }
            }
        } else {
            // Check if already in list to be safe (duplicate prevention should already catch this)
            if (session.driver?.idNumber != record.idNumber && session.passengers.none { it.idNumber == record.idNumber }) {
                session.passengers.add(record)
            }
        }
    }

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
