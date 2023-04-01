#ifndef __ANDROID__
#error Only Android platform is supported
#endif

#include "gmm.h"
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <amidi/AMidi.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "gdnative", __VA_ARGS__))

typedef struct gmm_platform
{
    JNIEnv * env;
    jobject activity;
} gmm_platform;

struct gmm_platform * gmm_platform_alloc()
{
    struct gmm_platform * platform = (struct gmm_platform *) get_core_api()->godot_alloc(sizeof(gmm_platform));
    memset(platform, 0x00, sizeof(gmm_platform));
    return platform;
}

void gmm_platform_free(struct gmm_platform * platform)
{
    get_core_api()->godot_free(platform);
}

bool gmm_platform_init(struct gmm_platform * platform)
{
    platform->env = get_android_api()->godot_android_get_env();
    platform->activity = get_android_api()->godot_android_get_activity();
    return platform->env != NULL && platform->activity != NULL ? true : false;
}

int gmm_platform_devices(struct gmm_data * gmm)
{
    JNIEnv * env = gmm->platform->env;

    jclass ActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");
    jclass Context = (*env)->FindClass(env, "android/content/Context");
    jclass MidiManager = (*env)->FindClass(env, "android/media/midi/MidiManager");

    jmethodID currentActivityThread = (*env)->GetStaticMethodID(env, ActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject cat = (*env)->CallStaticObjectMethod(env, ActivityThread, currentActivityThread);

    jmethodID getApplication = (*env)->GetMethodID(env, ActivityThread, "getApplication", "()Landroid/app/Application;");
    jobject app = (*env)->CallObjectMethod(env, cat, getApplication);

    jfieldID MIDI_SERVICE = (*env)->GetStaticFieldID(env, Context, "MIDI_SERVICE", "Ljava/lang/String;");
    jstring ms = (jstring) (*env)->GetStaticObjectField(env, Context, MIDI_SERVICE);

    jmethodID getSystemService = (*env)->GetMethodID(env, Context, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject mm = (*env)->CallObjectMethod(env, app, getSystemService, ms);

    jmethodID getDevices = (*env)->GetMethodID(env, MidiManager, "getDevices", "()[Landroid/media/midi/MidiDeviceInfo;");
    jobjectArray devices = (jobjectArray) (*env)->CallObjectMethod(env, mm, getDevices);

    jsize devicesLength = (*env)->GetArrayLength(env, devices);
    // strReturn = env->GetStringUTFChars(str_tm, 0);

    return (int)devicesLength;
}
