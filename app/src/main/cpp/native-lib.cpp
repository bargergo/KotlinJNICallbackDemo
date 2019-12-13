#include <jni.h>
#include <android/asset_manager_jni.h>
#include <string>
#include <vector>
#include "native-lib.h"
#include "AudioEngine.h"
#include "utils/logging_macros.h"
#include "callback/ObserverChain.h"

std::unique_ptr<AudioEngine> engine;

static JavaVM *jvm = nullptr;
std::vector<ObserverChain *> listeners;

JNIEnv *store_env;

extern "C" {

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_initEngine(
        JNIEnv *env,
        jobject /* this */,
        jobject jAssetManager) {

    AAssetManager *assetManager = AAssetManager_fromJava(env, jAssetManager);
    engine = std::make_unique<AudioEngine>(assetManager);
    engine->init();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_startEngine(
        JNIEnv *env,
        jobject /* this */) {

    engine->start();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_stopEngine(
        JNIEnv *env,
        jobject /* this */) {
    engine->stop();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_clap(
        JNIEnv *env,
        jobject /* this */) {
    engine->clap();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_loadClap(
        JNIEnv *env,
        jobject /* this */,
        jstring filename) {
    jboolean iscopy;
    const char *nativeString = env->GetStringUTFChars(filename, &iscopy);
    engine->loadClap(std::string{nativeString});
    env->ReleaseStringUTFChars(filename, nativeString);
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_loadBackingTrack(
        JNIEnv *env,
        jobject /* this */,
        jstring filename) {
    jboolean iscopy;
    const char *nativeString = env->GetStringUTFChars(filename, &iscopy);
    engine->loadBackingTrack(std::string{nativeString});
    env->ReleaseStringUTFChars(filename, nativeString);
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_playBackingTrack(
        JNIEnv *env,
        jobject /* this */) {
    engine->playBackingTrack();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_stopBackingTrack(
        JNIEnv *env,
        jobject /* this */) {
    engine->stopBackingTrack();
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_subscribe(JNIEnv *env, jobject instance,
                                                            jobject listener) {

    env->GetJavaVM(&jvm); //store jvm reference for later call

    store_env = env;

    jweak store_Wlistener = env->NewWeakGlobalRef(listener);
    jclass clazz = env->GetObjectClass(store_Wlistener);

    jmethodID onMusicStarted = env->GetMethodID(clazz, "onMusicStarted", "()V");
    jmethodID onMusicEnded = env->GetMethodID(clazz, "onMusicEnded", "()V");

    listeners.push_back(new ObserverChain(store_Wlistener, onMusicStarted, onMusicEnded));

    LOGV("GetEnv: Subscribe to Listener  OK ");
    if (nullptr == onMusicStarted)
        return;
}

JNIEXPORT void JNICALL
Java_hu_example_kotlinjnicallbackdemo_AudioEngine_deleteSubscriptions(JNIEnv *env,
                                                                      jobject instance) {
    if (!listeners.empty()) {
        for (auto &i : listeners) {
            env->DeleteWeakGlobalRef(i->listener);
            i->onMusicStarted = nullptr;
            i->onMusicEnded = nullptr;
        }
        listeners.clear();
    }
}

void call_listeners(JNIEnv *env, bool musicEnded) {
    for (auto &i : listeners) {
        if (musicEnded) {
            env->CallVoidMethod(i->listener,
                                i->onMusicEnded);
        } else {
            env->CallVoidMethod(i->listener,
                                i->onMusicStarted);
        }

    }
}

void callback_from_c(bool musicEnded) {
    LOGV("GetEnv: start Callback  to JNL");
    JNIEnv *g_env;
    if (nullptr == jvm) {
        LOGE("GetEnv: No VM");
        return;
    }
    //  double check it's all ok
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // set your JNI version
    args.name = nullptr; // you might want to give the java thread a name
    args.group = nullptr; // you might want to assign the java thread to a ThreadGroup

    int getEnvStat = jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        LOGE("GetEnv: not attached");
        if (jvm->AttachCurrentThread(&g_env, &args) != 0) {
            LOGE("GetEnv: Failed to attach");
        }
    } else if (getEnvStat == JNI_OK) {
        LOGV("GetEnv: JNI_OK");
    } else if (getEnvStat == JNI_EVERSION) {
        LOGE("GetEnv: version not supported");
    }

    call_listeners(g_env, musicEnded);

    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
    }

    if (getEnvStat == JNI_EDETACHED) {
        jvm->DetachCurrentThread();
    }
}
void musicEnded() {
    callback_from_c(true);
}

void musicStarted() {
    callback_from_c(false);
}

}
