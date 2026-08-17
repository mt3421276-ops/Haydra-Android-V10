#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>

#include "llama.h"

#define LOG_TAG "HAYDRA_NATIVE"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static llama_model *g_model = nullptr;
static llama_context *g_ctx = nullptr;
static llama_sampler *g_sampler = nullptr;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_loadModel(
        JNIEnv *env,
        jobject /* this */,
        jstring modelPath) {

    if (g_model != nullptr) {
        return env->NewStringUTF("OK: النموذج محمّل بالفعل");
    }

    const char *path =
        env->GetStringUTFChars(modelPath, nullptr);

    if (!path) {
        return env->NewStringUTF("ERROR: تعذر قراءة مسار النموذج");
    }

    LOGI("STEP 1: llama_backend_init START");
    llama_backend_init();
    LOGI("STEP 1: llama_backend_init DONE");

    llama_model_params model_params =
        llama_model_default_params();

    LOGI("STEP 2: llama_model_load_from_file START");
    LOGI("MODEL PATH: %s", path);
    g_model = llama_model_load_from_file(
        path,
        model_params
    );
    LOGI("STEP 2: llama_model_load_from_file DONE");

    env->ReleaseStringUTFChars(modelPath, path);

    if (!g_model) {
        return env->NewStringUTF(
            "ERROR: فشل تحميل ملف النموذج"
        );
    }

    llama_context_params ctx_params =
        llama_context_default_params();

    ctx_params.n_ctx = 2048;
    ctx_params.n_batch = 512;
    ctx_params.n_threads = 4;
    ctx_params.n_threads_batch = 4;

    LOGI("STEP 3: llama_init_from_model START");
    g_ctx = llama_init_from_model(
        g_model,
        ctx_params
    );
    LOGI("STEP 3: llama_init_from_model DONE");

    if (!g_ctx) {
        llama_model_free(g_model);
        g_model = nullptr;

        return env->NewStringUTF(
            "ERROR: فشل إنشاء سياق النموذج"
        );
    }

    llama_sampler_chain_params sampler_params =
        llama_sampler_chain_default_params();

    LOGI("STEP 4: llama_sampler_chain_init START");
    g_sampler =
        llama_sampler_chain_init(sampler_params);
    LOGI("STEP 4: llama_sampler_chain_init DONE");

    llama_sampler_chain_add(
        g_sampler,
        llama_sampler_init_greedy()
    );

    return env->NewStringUTF(
        "OK: تم تحميل Haydra Qwen بنجاح"
    );
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_haydra_v10_MainActivity_processMessage(
        JNIEnv *env,
        jobject /* this */,
        jstring message) {

    if (!g_model || !g_ctx || !g_sampler) {
        return env->NewStringUTF(
            "🐉 Haydra: النموذج غير محمّل."
        );
    }

    const char *input =
        env->GetStringUTFChars(message, nullptr);

    if (!input) {
        return env->NewStringUTF(
            "🐉 Haydra: تعذر قراءة الرسالة."
        );
    }

    std::string prompt =
        "<|im_start|>system\n"
        "You are Haydra, a helpful local AI assistant. "
        "Answer clearly and concisely.\n"
        "<|im_end|>\n"
        "<|im_start|>user\n";

    prompt += input;

    prompt +=
        "\n<|im_end|>\n"
        "<|im_start|>assistant\n";

    env->ReleaseStringUTFChars(message, input);

    const llama_vocab *vocab =
        llama_model_get_vocab(g_model);

    int32_t n_tokens = llama_tokenize(
        vocab,
        prompt.c_str(),
        (int32_t)prompt.size(),
        nullptr,
        0,
        true,
        true
    );

    if (n_tokens <= 0) {
        return env->NewStringUTF(
            "🐉 Haydra: فشل تحويل النص إلى tokens."
        );
    }

    std::vector<llama_token> tokens(n_tokens);

    n_tokens = llama_tokenize(
        vocab,
        prompt.c_str(),
        (int32_t)prompt.size(),
        tokens.data(),
        n_tokens,
        true,
        true
    );

    if (n_tokens <= 0) {
        return env->NewStringUTF(
            "🐉 Haydra: فشل tokenize."
        );
    }

    llama_batch batch =
        llama_batch_get_one(
            tokens.data(),
            n_tokens
        );

    int result =
        llama_decode(g_ctx, batch);

    if (result != 0) {
        return env->NewStringUTF(
            "🐉 Haydra: فشل decode."
        );
    }

    std::string output;

    for (int i = 0; i < 128; ++i) {

        llama_token token =
            llama_sampler_sample(
                g_sampler,
                g_ctx,
                -1
            );

        llama_sampler_accept(
            g_sampler,
            token
        );

        if (llama_vocab_is_eog(
                vocab,
                token)) {
            break;
        }

        char piece[256];

        int32_t len =
            llama_token_to_piece(
                vocab,
                token,
                piece,
                sizeof(piece),
                0,
                true
            );

        if (len > 0) {
            output.append(piece, len);
        }

        llama_batch next =
            llama_batch_get_one(
                &token,
                1
            );

        result =
            llama_decode(
                g_ctx,
                next
            );

        if (result != 0) {
            break;
        }
    }

    if (output.empty()) {
        output =
            "🐉 Haydra: لم ينتج النموذج ردًا.";
    }

    return env->NewStringUTF(
        output.c_str()
    );
}
