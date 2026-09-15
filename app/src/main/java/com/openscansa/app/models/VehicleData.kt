package com.openscansa.app.models

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class VehicleData(
    @SerialName("control_code")
    val controlCode: String? = null,
    @SerialName("licence_code")
    val licenceCode: String? = null,
    @SerialName("register_number")
    val registerNumber: String? = null,
    @SerialName("sequence_number")
    val sequenceNumber: String? = null,
    @SerialName("control_number")
    val controlNumber: String? = null,
    @SerialName("licence_number")
    val licenceNumber: String? = null,
    @SerialName("vehicle_register_number")
    val vehicleRegisterNumber: String? = null,
    @SerialName("vehicle_type")
    val vehicleType: String? = null,
    val make: String? = null,
    val model: String? = null,
    val colour: String? = null,
    val vin: String? = null,
    @SerialName("engine_number")
    val engineNumber: String? = null,
    @SerialName("expiry_date")
    val expiryDate: String? = null,
    @SerialName("raw_data")
    val rawData: String? = null
)
