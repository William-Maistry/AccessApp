package com.openscansa.app.models

import kotlinx.serialization.SerialName
import kotlinx.serialization.Serializable

@Serializable
data class IdData(
    @SerialName("id_number")
    val idNumber: String,
    val surname: String? = null,
    @SerialName("first_names")
    val firstNames: String? = null,
    val sex: String? = null,
    val nationality: String? = null,
    val dob: String? = null,
    @SerialName("citizenship_country")
    val citizenshipCountry: String? = null,
    @SerialName("citizen_type")
    val citizenType: String? = null,
    @SerialName("card_issue_date")
    val cardIssueDate: String? = null,
    @SerialName("issuing_office_code")
    val issuingOfficeCode: String? = null,
    @SerialName("internal_record_number")
    val internalRecordNumber: String? = null,
    @SerialName("is_parsed")
    val isParsed: Boolean = false,
    @SerialName("raw_data")
    val rawData: String? = null
)
