package com.openscansa.app.models

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class AttendanceSession(
    val scanType: String, // "in" or "out"
    var arrivalType: String? = null, // "vehicle", "pedestrian"
    var participantType: String? = null, // "staff", "visitor"
    var driver: StaffRecord? = null,
    val passengers: MutableList<StaffRecord> = mutableListOf(),
    var vehicle: VehicleData? = null,
    var vehicleOwnerName: String? = null,
    var isVehicleMatched: Boolean = false,
    var needsNewQrCode: Boolean = false
)

@Serializable
data class StaffRecord(
    val id: String,
    @SerialName("first_names")
    val firstNames: String,
    @SerialName("last_name")
    val lastName: String,
    @SerialName("id_number")
    val idNumber: String,
    @SerialName("licence_number")
    val licenceNumber: String? = null,
    val passcode: String? = null,
    var missingStateWarning: String? = null
)

@Serializable
data class AccessLog(
    val id: String? = null,
    @SerialName("profile_id") val profileId: String,
    @SerialName("scan_time") val scanTime: String? = null,
    @SerialName("scan_type") val scanType: String,
    @SerialName("first_names") val firstNames: String? = null,
    @SerialName("last_name") val lastName: String? = null,
    @SerialName("reissue_qr") val reissueQr: Boolean = false
)

@Serializable
data class AccessLogInsert(
    @SerialName("profile_id") val profileId: String,
    @SerialName("scan_type") val scanType: String,
    @SerialName("reissue_qr") val reissueQr: Boolean = false
)

@Serializable
data class AccessLogSummary(
    @SerialName("scan_type") val scanType: String
)
