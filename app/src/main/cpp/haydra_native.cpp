#include <jni.h>
#include <dlfcn.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_processMessage(
        JNIEnv *env,
        jobject /* this */,
        jstring message) {

    void *handle = dlopen("libllama.so.0", RTLD_NOW);

    if (handle == nullptr) {
        return env->NewStringUTF(
            "🐉 Haydra: فشل تحميل محرك llama.cpp"
        );
    }

    dlclose(handle);

    const char *text =
        "🐉 Haydra Native: تم تحميل محرك llama.cpp بنجاح!";

    return env->NewStringUTF(text);
}
