package com.openscansa.app.camera

object NativeScanner {

    init {
        System.loadLibrary("native-scanner")
    }

    external fun decodePDF417(
        image: ByteArray,
        width: Int,
        height: Int
    ): String?

    external fun decodePDF417Bytes(
        image: ByteArray,
        width: Int,
        height: Int
    ): ByteArray?
}