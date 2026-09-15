package com.openscansa.app.utils

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow

object DebugConsole {

    private const val MAX_LINES = 100

    private val _messages = MutableStateFlow("")
    val messages: StateFlow<String> = _messages

    private val lines = mutableListOf<String>()

    fun log(message: String) {
        val timestamp = java.text.SimpleDateFormat(
            "HH:mm:ss",
            java.util.Locale.getDefault()
        ).format(java.util.Date())

        lines.add("[$timestamp] $message")

        while (lines.size > MAX_LINES) {
            lines.removeAt(0)
        }

        _messages.value = lines.joinToString("\n")
    }

    fun clear() {
        lines.clear()
        _messages.value = ""
    }
}