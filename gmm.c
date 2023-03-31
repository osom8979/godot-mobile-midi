#include "gmm.h"

#include <assert.h>
#include <string.h>

#if defined(__ANDROID__)
# include "gmm-android.h"
#elif defined(__APPLE__)
# include "TargetConditionals.h"
# if TARGET_OS_IPHONE
#  include "gmm-ios.h"
# endif
#endif

const godot_gdnative_core_api_struct * g_api = NULL;
const godot_gdnative_ext_nativescript_api_struct * g_nativescript_api = NULL;
const godot_gdnative_ext_android_api_struct * g_android_api = NULL;

bool is_android()
{
#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

bool is_ios()
{
#if defined(__APPLE__) && defined(TARGET_OS_IPHONE)
    return true;
#else
    return false;
#endif
}

char const * const get_platform_name()
{
    if (is_android()) {
        return "ANDROID";
    } else if (is_ios()) {
        return  "IOS";
    } else {
        return "Desktop";
    }
}

void set_api(const godot_gdnative_core_api_struct * value)
{
    g_api = value;
}

void set_nativescript_api(const godot_gdnative_ext_nativescript_api_struct * value)
{
    g_nativescript_api = value;
}

void set_android_api(const godot_gdnative_ext_android_api_struct * value)
{
    g_android_api = value;
}

const godot_gdnative_core_api_struct * get_api()
{
    return g_api;
}

const godot_gdnative_ext_nativescript_api_struct * get_nativescript_api()
{
    return g_nativescript_api;
}

const godot_gdnative_ext_android_api_struct * get_android_api()
{
    return g_android_api;
}

void clear_apis()
{
    g_api = NULL;
    g_nativescript_api = NULL;
    g_android_api = NULL;
}

// ----------
// Interfaces
// ----------

void * gmm_new(godot_object * obj, void * method)
{
    assert(g_api != NULL);
    gmm_data * user = (gmm_data*) g_api->godot_alloc(sizeof(gmm_data));
    memset(user, 0x00, sizeof(gmm_data));

    strncpy(user->platform, get_platform_name(), MAX_PLATFORM_LENGTH);

    return (void*)user;
}

void gmm_del(godot_object * obj, void * method, void * user)
{
    assert(g_api != NULL);
    assert(user != NULL);
    g_api->godot_free(user);
}

godot_variant gmm_get_platform_name(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** argv)
{
    assert(g_api != NULL);
    assert(user != NULL);
    gmm_data * data = (gmm_data*)user;

    // jclass ctx = env->FindClass("android/content/Context");
    // jfieldID fid = env->GetStaticFieldID(ctx,"TELEPHONY_SERVICE","Ljava/lang/String;");
    // jstring str = (jstring) env->GetStaticObjectField(ctx, fid);
    // jmethodID mid = env->GetMethodID(ctx, "getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
    // jobject tm = env->CallObjectMethod(context, mid, str);

    // jclass ctx_tm = env->FindClass("android/telephony/TelephonyManager");
    // jmethodID mid_tm = env->GetMethodID(ctx_tm,"getDeviceId","()Ljava/lang/String;");
    // jstring str_tm = (jstring) env->CallObjectMethod(tm, mid_tm);

    // strReturn = env->GetStringUTFChars(str_tm, 0);

    // if (g_android_api != NULL) {
    //     void * env = g_android_api->godot_android_get_env();
    //     if (env != NULL) {
    //         strcpy(data->data, "Hello, World 2");
    //     } else {
    //         strcpy(data->data, "Hello, World 1");
    //     }
    // } else {
    //     strcpy(data->data, "Hello, World 0");
    // }

    godot_string text;
    g_api->godot_string_new(&text);
    g_api->godot_string_parse_utf8(&text, data->platform);

    godot_variant result;
    g_api->godot_variant_new_string(&result, &text);

    g_api->godot_string_destroy(&text);

    return result;
}

godot_variant gmm_init(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** argv)
{
    assert(g_api != NULL);
    assert(user != NULL);
    assert(argc >= 1);
    assert(argv[0] != NULL);
    gmm_data * data = (gmm_data*)user;

    g_api->godot_variant_new_copy(&data->self, argv[0]);

    godot_variant ret;
    g_api->godot_variant_new_bool(&ret, GODOT_TRUE);
    return ret;
}

