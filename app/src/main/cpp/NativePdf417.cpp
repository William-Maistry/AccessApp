#include <jni.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_openscansa_app_camera_NativePdf417_test(JNIEnv* env, jobject /* this */) {
    return env->NewStringUTF("Native library loaded successfully!");
}