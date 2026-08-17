#include <jni.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_getNativeMessage(
        JNIEnv *env,
        jobject /* this */) {

    return env->NewStringUTF("🐉 Haydra Native Engine V11 يعمل!");
}
