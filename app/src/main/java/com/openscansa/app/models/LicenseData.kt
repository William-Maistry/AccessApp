package com.openscansa.app.models

import com.openscansa.app.utils.PostgrestByteaSerializer
import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class LicenseData(
    @SerialName("id_number")
    val idNumber: String,
    val surname: String? = null,
    val initials: String? = null,
    @SerialName("license_number")
    val licenseNumber: String? = null,
    @SerialName("issue_number")
    val issueNumber: String? = null,
    @SerialName("country_of_issue")
    val countryOfIssue: String? = null,
    @SerialName("birth_date")
    val birthDate: String? = null,
    @SerialName("valid_from")
    val validFrom: String? = null,
    @SerialName("valid_to")
    val validTo: String? = null,
    val gender: String? = null,
    @SerialName("vehicle_codes")
    val vehicleCodes: String? = null,
    @SerialName("vehicle_restrictions")
    val vehicleRestrictions: String? = null,
    @SerialName("driver_restriction")
    val driverRestriction: String? = null,
    @SerialName("pdp_code")
    val pdpCode: String? = null,
    @SerialName("pdp_expiry")
    val pdpExpiry: String? = null,
    @SerialName("photo_bytes")
    @Serializable(with = PostgrestByteaSerializer::class)
    val photoBytes: ByteArray? = null,
    @SerialName("raw_data")
    val rawData: String? = null
)
