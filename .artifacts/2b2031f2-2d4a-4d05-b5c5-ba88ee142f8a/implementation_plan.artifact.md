# Implementation Plan - Fix Photo Decoding and Robust License Detection (Iteration 4)

I will integrate the actual South African Driver's License decompression logic provided in `SWIDecoder.cpp` and fix the payload normalization issue to ensure the license header is correctly identified even when null bytes are corrupted to `0x3F`.

## User Review Required

> [!IMPORTANT]
> I am replacing the placeholder C++ logic with the full implementation from the `SWIDecoder.cpp` file you provided. This is a significant change to the native layer and will require a full rebuild of the project.

## Proposed Changes

### Native Layer Update

#### [MODIFY] [DecoderJni.cpp](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/cpp/DecoderJni.cpp)
- Replace the entire file with the content of `SWIDecoder.cpp` provided by the user.
- Ensure all required headers (`SWIDecoder.h`, `jni.h`, etc.) are correctly referenced.
- This will enable the real `getDecodedPhoto` logic.

### Robust Detection & Normalization

#### [MODIFY] [CameraScanner.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/camera/CameraScanner.kt)
- **Pre-Normalization**: Apply `normalizeLicenseBytes` to the raw scan *before* searching for the header.
- **Fixed Recovery Logic**: Update `findSaDlPayload` to correctly handle `0x3F` corruption. Since `0x3F` typically replaces `0x00`, it shouldn't affect the `0x01 0x9B` signature directly, but it affects the version check and the payload itself.
- **Improved Fallback**: Ensure the raw hex dump shown in the UI uses the `normalizedBytes` and does NOT have null bytes stripped, so you can see the `00` bytes in the hex dump.

#### [MODIFY] [BarcodeParser.kt](file:///C:/Android Studio Gemini Test/Scanner/app/src/main/java/com/openscansa/app/parser/BarcodeParser.kt)
- Update the diagnostic display to show a clearer "Normalized Header" view.

## Verification Plan

### Manual Verification
- Deploy and scan.
- Verify that the photo now appears (thanks to the real decompression logic).
- Verify that the hex dump in the diagnostic view shows `00` instead of `3F` if normalization is working.
- Verify if structured license data finally appears.
