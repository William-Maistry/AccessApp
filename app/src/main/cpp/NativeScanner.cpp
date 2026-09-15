#include <jni.h>
#include <string>

#include "scanner.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_openscansa_app_camera_NativeScanner_decodePDF417(
        JNIEnv* env,
        jobject,
        jbyteArray imageData,
        jint width,
        jint height)
{
    if (imageData == nullptr)
        return nullptr;

    jbyte* bytes = env->GetByteArrayElements(imageData, nullptr);

    std::string result = DecodePDF417(
            reinterpret_cast<uint8_t*>(bytes),
            width,
            height);

    env->ReleaseByteArrayElements(imageData, bytes, JNI_ABORT);

    if (result.empty())
        return nullptr;

    return env->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_openscansa_app_camera_NativeScanner_decodePDF417Bytes(
        JNIEnv* env,
        jobject,
        jbyteArray imageData,
        jint width,
        jint height)
{
    if (imageData == nullptr)
        return nullptr;

    jbyte* bytes = env->GetByteArrayElements(imageData, nullptr);
    std::vector<uint8_t> decoded = DecodePDF417Bytes(
            reinterpret_cast<uint8_t*>(bytes),
            width,
            height);
    env->ReleaseByteArrayElements(imageData, bytes, JNI_ABORT);

    if (decoded.empty())
        return nullptr;

    jbyteArray resultArray = env->NewByteArray(static_cast<jsize>(decoded.size()));
    if (resultArray == nullptr)
        return nullptr;

    env->SetByteArrayRegion(resultArray, 0, static_cast<jsize>(decoded.size()), reinterpret_cast<const jbyte*>(decoded.data()));
    return resultArray;
}