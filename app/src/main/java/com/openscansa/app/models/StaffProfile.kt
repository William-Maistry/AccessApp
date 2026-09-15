package com.openscansa.app.models

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class StaffProfile(
    @SerialName("first_names")
    val firstNames: String,
    @SerialName("last_name")
    val lastName: String,
    @SerialName("id_number")
    val idNumber: String? = null,
    @SerialName("licence_number")
    val licenceNumber: String? = null,
    val gender: String,
    @SerialName("phone_number")
    val phoneNumber: String,
    @SerialName("secondary_phone")
    val secondaryPhone: String? = null,
    val email: String? = null,
    val position: String,
    val passcode: String? = null,
    @SerialName("qr_code")
    val qrCode: String? = null
)
