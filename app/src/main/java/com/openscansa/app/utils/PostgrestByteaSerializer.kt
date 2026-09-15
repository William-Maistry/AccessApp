package com.openscansa.app.utils

import kotlinx.serialization.KSerializer
import kotlinx.serialization.descriptors.PrimitiveKind
import kotlinx.serialization.descriptors.PrimitiveSerialDescriptor
import kotlinx.serialization.descriptors.SerialDescriptor
import kotlinx.serialization.encoding.Decoder
import kotlinx.serialization.encoding.Encoder

object PostgrestByteaSerializer : KSerializer<ByteArray> {
    override val descriptor: SerialDescriptor = 
        PrimitiveSerialDescriptor("PostgrestBytea", PrimitiveKind.STRING)

    private val hexChars = "0123456789abcdef".toCharArray()

    override fun serialize(encoder: Encoder, value: ByteArray) {
        // High-performance hex encoding with Postgres \x prefix
        val result = StringBuilder(value.size * 2 + 2)
        result.append("\\x")
        for (b in value) {
            val i = b.toInt() and 0xFF
            result.append(hexChars[i shr 4])
            result.append(hexChars[i and 0x0F])
        }
        encoder.encodeString(result.toString())
    }

    override fun deserialize(decoder: Decoder): ByteArray {
        val hexString = decoder.decodeString()
        val pureHex = if (hexString.startsWith("\\x")) hexString.substring(2) else hexString
        if (pureHex.isEmpty()) return ByteArray(0)
        
        return pureHex.chunked(2)
            .map { it.toInt(16).toByte() }
            .toByteArray()
    }
}
