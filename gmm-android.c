#ifndef __ANDROID__
#error Only Android platform is supported
#endif

#include "gmm.h"
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <amidi/AMidi.h>

// #define HAS_ACTIVITY_ATTRIBUTE

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "gmm", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "gmm", __VA_ARGS__))

// -----------
// JNI Helpers
// -----------

void _throw_exception(JNIEnv * env, const char * message)
{
    jclass Exception = (*env)->FindClass(env, "java/lang/Exception");
    if (Exception == NULL) {
        LOGE("Could not find exception class to throw error");
        return;
    }
    (*env)->ThrowNew(env, Exception, message);
}

jobjectArray _get_midi_devices(JNIEnv * env)
{
    jclass ActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");
    jclass Context = (*env)->FindClass(env, "android/content/Context");
    jclass MidiManager = (*env)->FindClass(env, "android/media/midi/MidiManager");

    jmethodID currentActivityThread = (*env)->GetStaticMethodID(
            env,
            ActivityThread,
            "currentActivityThread",
            "()Landroid/app/ActivityThread;");
    jobject currentThread = (*env)->CallStaticObjectMethod(
            env,
            ActivityThread,
            currentActivityThread);

    jmethodID getApplication = (*env)->GetMethodID(
            env,
            ActivityThread,
            "getApplication",
            "()Landroid/app/Application;");
    jobject application = (*env)->CallObjectMethod(
            env,
            currentThread,
            getApplication);

    jfieldID MIDI_SERVICE = (*env)->GetStaticFieldID(
            env,
            Context,
            "MIDI_SERVICE",
            "Ljava/lang/String;");
    jstring midiService = (jstring) (*env)->GetStaticObjectField(
            env,
            Context,
            MIDI_SERVICE);

    jmethodID getSystemService = (*env)->GetMethodID(
            env,
            Context,
            "getSystemService",
            "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject midiManager = (*env)->CallObjectMethod(
            env,
            application,
            getSystemService,
            midiService);

    jmethodID getDevices = (*env)->GetMethodID(
            env,
            MidiManager,
            "getDevices",
            "()[Landroid/media/midi/MidiDeviceInfo;");
    jobjectArray devices = (jobjectArray) (*env)->CallObjectMethod(
            env,
            midiManager,
            getDevices);

    (*env)->DeleteLocalRef(env, ActivityThread);
    (*env)->DeleteLocalRef(env, Context);
    (*env)->DeleteLocalRef(env, MidiManager);

    (*env)->DeleteLocalRef(env, currentThread);
    (*env)->DeleteLocalRef(env, application);
    (*env)->DeleteLocalRef(env, midiService);
    (*env)->DeleteLocalRef(env, midiManager);

    return devices;
}

typedef struct gmm_platform
{
    JNIEnv * env;
#if defined(HAS_ACTIVITY_ATTRIBUTE)
    jobject activity;
#endif
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
    JNIEnv * env = get_android_api()->godot_android_get_env();
    platform->env = env;

#if defined(HAS_ACTIVITY_ATTRIBUTE)
    platform->activity = get_android_api()->godot_android_get_activity();
    return platform->env != NULL && platform->activity != NULL ? true : false;
#else
    return platform->env != NULL ? true : false;
#endif
}

struct gmm_device_info * gmm_platform_devices(struct gmm_data * gmm, unsigned * length)
{
    JNIEnv * env = gmm->platform->env;
    struct gmm_device_info * result = NULL;

    jobjectArray devices = _get_midi_devices(env);
    jsize devicesLength = (*env)->GetArrayLength(env, devices);

    if (length != NULL) {
        *length = (unsigned)devicesLength;
    }

    if (devicesLength == 0) {
        (*env)->DeleteLocalRef(env, devices);
        return result;
    }

    jclass MidiDeviceInfo = (*env)->FindClass(env, "android/media/midi/MidiDeviceInfo");
    jclass Bundle = (*env)->FindClass(env, "android/os/Bundle");

    result = get_core_api()->godot_alloc(sizeof(struct gmm_device_info) * devicesLength);
    for (jsize i = 0; i < devicesLength; ++i) {
        jobject device = (*env)->GetObjectArrayElement(env, devices, i);

        jmethodID getInputPortCount = (*env)->GetMethodID(
                env,
                MidiDeviceInfo,
                "getInputPortCount",
                "()I");
        jmethodID getOutputPortCount = (*env)->GetMethodID(
                env,
                MidiDeviceInfo,
                "getOutputPortCount",
                "()I");

        jint inCount = (jint) (*env)->CallIntMethod(env, device, getInputPortCount);
        jint outCount = (jint) (*env)->CallIntMethod(env, device, getOutputPortCount);

        (result + i)->input_count = (int)inCount;
        (result + i)->output_count = (int)outCount;

        jmethodID getProperties = (*env)->GetMethodID(
                env,
                MidiDeviceInfo,
                "getProperties",
                "()Landroid/os/Bundle;");
        jobject properties = (jobject) (*env)->CallObjectMethod(
                env,
                device,
                getProperties);

        jfieldID PROPERTY_MANUFACTURER = (*env)->GetStaticFieldID(
                env,
                MidiDeviceInfo,
                "PROPERTY_MANUFACTURER",
                "Ljava/lang/String;");
        jstring propertyManufacturer = (jstring) (*env)->GetStaticObjectField(
                env,
                MidiDeviceInfo,
                PROPERTY_MANUFACTURER);

        jfieldID PROPERTY_NAME = (*env)->GetStaticFieldID(
                env,
                MidiDeviceInfo,
                "PROPERTY_NAME",
                "Ljava/lang/String;");
        jstring propertyName = (jstring) (*env)->GetStaticObjectField(
                env,
                MidiDeviceInfo,
                PROPERTY_NAME);

        jmethodID getString = (*env)->GetMethodID(
                env,
                Bundle,
                "getString",
                "(Ljava/lang/String;)Ljava/lang/String;");
        jstring manufacturer = (jstring) (*env)->CallObjectMethod(
                env,
                properties,
                getString,
                propertyManufacturer);
        jstring name = (jstring) (*env)->CallObjectMethod(
                env,
                properties,
                getString,
                propertyName);

        const char * rawManufacturer = (*env)->GetStringUTFChars(env, manufacturer, NULL);
        strncpy(result[i].manufacturer, rawManufacturer, MAX_DEVICE_MANUFACTURER_LENGTH);

        const char * rawName = (*env)->GetStringUTFChars(env, name, NULL);
        strncpy(result[i].name, rawName, MAX_DEVICE_NAME_LENGTH);

        (*env)->DeleteLocalRef(env, manufacturer);
        (*env)->DeleteLocalRef(env, propertyManufacturer);
        (*env)->DeleteLocalRef(env, properties);
        (*env)->DeleteLocalRef(env, device);
    }

    (*env)->DeleteLocalRef(env, devices);
    (*env)->DeleteLocalRef(env, MidiDeviceInfo);
    (*env)->DeleteLocalRef(env, Bundle);

    return result;
}

