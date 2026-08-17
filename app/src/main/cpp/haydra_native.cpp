#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_getNativeMessage(
        JNIEnv *env,
        jobject /* this */) {

    return env->NewStringUTF(
            "🐉 Haydra Native Engine V11 يعمل!"
    );
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_processMessage(
        JNIEnv *env,
        jobject /* this */,
        jstring message) {

    const char *input = env->GetStringUTFChars(message, nullptr);

    std::string reply =
            "🐉 Haydra Native: وصلتني رسالتك: ";

    reply += input;

    env->ReleaseStringUTFChars(message, input);

    return env->NewStringUTF(reply.c_str());
}
