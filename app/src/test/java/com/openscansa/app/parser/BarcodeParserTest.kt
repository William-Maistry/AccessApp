package com.openscansa.app.parser

import com.openscansa.app.camera.decodePayloadText
import org.junit.Assert.assertFalse
import org.junit.Assert.assertTrue
import org.junit.Test

class BarcodeParserTest {
    @Test
    fun `parse preserves decoded south african licence fields for display`() {
        val raw = """
            Decoded SA DL:
            License type 1: B
            Surname: Smith
            Initials: J
            ID Number: 8001011234089
            License Number: 123456789
            Valid To: 20301231
        """.trimIndent()

        val result = BarcodeParser.parse(raw, "PDF417")

        assertTrue(result.displayValue.contains("Surname: Smith"))
        assertTrue(result.displayValue.contains("License Number: 123456789"))
        assertTrue(result.displayValue.contains("Valid To: 20301231"))
    }

    @Test
    fun `decode payload text avoids hex dump for printable bytes`() {
        val payload = "Decoded SA DL:\nLicense Number: 123456789\nValid To: 20301231".toByteArray(Charsets.UTF_8)
        val text = decodePayloadText(payload)

        assertTrue(text.contains("License Number: 123456789"))
        assertFalse(text.contains("4E 61 74"))
    }
}
