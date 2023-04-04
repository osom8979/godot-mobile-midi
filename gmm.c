#include "gmm.h"

#include <assert.h>
#include <string.h>
#include <stdarg.h>

// #if defined(__ANDROID__)
// # include "gmm-android.h"
// #elif defined(__APPLE__)
// # include "TargetConditionals.h"
// # if TARGET_OS_IPHONE
// #  include "gmm-ios.h"
// # endif
// #endif

const struct godot_gdnative_core_api_struct * g_core_api = NULL;
const struct godot_gdnative_ext_nativescript_api_struct * g_nativescript_api = NULL;
const struct godot_gdnative_ext_android_api_struct * g_android_api = NULL;

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

int get_platform_index()
{
#if defined(__ANDROID__)
    return PLATFORM_INDEX_ANDROID;
#elif defined(__APPLE__)
# if defined(TARGET_OS_IPHONE)
    return PLATFORM_INDEX_IOS;
# else
    return PLATFORM_INDEX_MAC;
# endif
#else
    return PLATFORM_INDEX_LINUX;
#endif
}

void set_core_api(const struct godot_gdnative_core_api_struct * value)
{
    g_core_api = value;
}

void set_nativescript_api(const struct godot_gdnative_ext_nativescript_api_struct * value)
{
    g_nativescript_api = value;
}

void set_android_api(const struct godot_gdnative_ext_android_api_struct * value)
{
    g_android_api = value;
}

const struct godot_gdnative_core_api_struct * get_core_api()
{
    return g_core_api;
}

const struct godot_gdnative_ext_nativescript_api_struct * get_nativescript_api()
{
    return g_nativescript_api;
}

const struct godot_gdnative_ext_android_api_struct * get_android_api()
{
    return g_android_api;
}

void clear_apis()
{
    g_core_api = NULL;
    g_nativescript_api = NULL;
    g_android_api = NULL;
}

// -------------
// Internal APIs
// -------------

void _print(const char * message)
{
    godot_string str;
    g_core_api->godot_string_new(&str);
    g_core_api->godot_string_parse_utf8(&str, message);
    g_core_api->godot_print(&str);
    g_core_api->godot_string_destroy(&str);
}

void _printf(const char * format, ...)
{
    char buffer[2048] = {0,};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 2048, format, args);
    va_end(args);

    _print(buffer);
}

bool _call_on_read_midi(struct gmm_data * gmm, int v0, int v1)
{
    godot_variant arg0;
    godot_variant arg1;

    godot_variant const * args[2];
    args[0] = &arg0;
    args[1] = &arg1;

    godot_variant_call_error err;

    g_core_api->godot_variant_new_int(&arg0, v0);
    g_core_api->godot_variant_new_int(&arg1, v1);
    g_core_api->godot_variant_call(
            &gmm->self,
            &gmm->on_read_midi_func_name,
            args,
            2,
            &err);

    return err.error == GODOT_CALL_ERROR_CALL_OK ? true : false;
}

// ----------
// Interfaces
// ----------

void * gmm_new(godot_object * obj, void * method)
{
    assert(g_core_api != NULL);

    gmm_data * gmm = (gmm_data*) g_core_api->godot_alloc(sizeof(gmm_data));
    memset(gmm, 0x00, sizeof(gmm_data));

    strncpy(gmm->platform_name, get_platform_name(), MAX_PLATFORM_NAME_LENGTH);
    gmm->platform = gmm_platform_alloc();
    gmm->call_on_read_midi = &_call_on_read_midi;
    gmm->print = &_print;
    gmm->printf = &_printf;

    return (void*)gmm;
}

void gmm_del(godot_object * obj, void * method, void * user)
{
    assert(g_core_api != NULL);
    assert(user != NULL);

    gmm_data * gmm = (gmm_data*)user;

    g_core_api->godot_variant_destroy(&gmm->self);
    g_core_api->godot_string_destroy(&gmm->on_read_midi_func_name);

    gmm_platform_free(gmm->platform);
    g_core_api->godot_free(gmm);
}

godot_variant gmm_get_platform_name(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** argv)
{
    assert(g_core_api != NULL);
    assert(user != NULL);
    gmm_data * gmm = (gmm_data*)user;

    // jclass ctx = env->FindClass("android/content/Context");
    // jfieldID fid = env->GetStaticFieldID(ctx,"TELEPHONY_SERVICE","Ljava/lang/String;");
    // jstring str = (jstring) env->GetStaticObjectField(ctx, fid);
    // jmethodID mid = env->GetMethodID(ctx, "getSystemService","(Ljava/lang/String;)Ljava/lang/Object;");
    // jobject tm = env->CallObjectMethod(context, mid, str);

    // jclass ctx_tm = env->FindClass("android/telephony/TelephonyManager");
    // jmethodID mid_tm = env->GetMethodID(ctx_tm,"getDeviceId","()Ljava/lang/String;");
    // jstring str_tm = (jstring) env->CallObjectMethod(tm, mid_tm);

    // strReturn = env->GetStringUTFChars(str_tm, 0);

    godot_string text;
    g_core_api->godot_string_new(&text);
    g_core_api->godot_string_parse_utf8(&text, gmm->platform_name);

    godot_variant result;
    g_core_api->godot_variant_new_string(&result, &text);

    g_core_api->godot_string_destroy(&text);

    return result;
}

godot_variant gmm_init(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** argv)
{
    assert(g_core_api != NULL);
    assert(user != NULL);
    assert(argc >= 1);
    assert(argv[0] != NULL);
    gmm_data * gmm = (gmm_data*)user;

    g_core_api->godot_variant_new_copy(&gmm->self, argv[0]);

    godot_string read_cb_name = g_core_api->godot_variant_as_string(argv[1]);
    g_core_api->godot_string_new_copy(&gmm->on_read_midi_func_name, &read_cb_name);

    bool platform_init_result = gmm_platform_init(gmm->platform);
    godot_variant ret;
    g_core_api->godot_variant_new_bool(&ret, platform_init_result ? GODOT_TRUE : GODOT_FALSE);
    return ret;
}

godot_variant gmm_get_devices(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** args)
{
    assert(g_core_api != NULL);
    assert(user != NULL);
    gmm_data * gmm = (gmm_data*)user;

    unsigned device_length = 0;
    struct gmm_device_info * devices = gmm_platform_devices(gmm, &device_length);

    godot_dictionary dic;
    g_core_api->godot_dictionary_new(&dic);

    if (device_length == 0) {
        assert(devices == NULL);

        godot_variant ret;
        g_core_api->godot_variant_new_dictionary(&ret, &dic);
        return ret;
    }

    assert(device_length >= 0);
    assert(devices != NULL);

    for (unsigned i = 0; i < device_length; ++i) {
        // Device Manufacturer
        godot_string device_manufacturer_key;
        godot_string device_manufacturer_val;
        g_core_api->godot_string_new(&device_manufacturer_key);
        g_core_api->godot_string_new(&device_manufacturer_val);
        g_core_api->godot_string_parse_utf8(&device_manufacturer_key, "manufacturer");
        g_core_api->godot_string_parse_utf8(&device_manufacturer_val, (devices + i)->manufacturer);
        // Device Manufacturer - Variant
        godot_variant device_manufacturer_var_key;
        godot_variant device_manufacturer_var_val;
        g_core_api->godot_variant_new_string(&device_manufacturer_var_key, &device_manufacturer_key);
        g_core_api->godot_variant_new_string(&device_manufacturer_var_val, &device_manufacturer_val);
        g_core_api->godot_dictionary_set(&dic, &device_manufacturer_var_key, &device_manufacturer_var_val);

        // Device Name
        godot_string device_name_key;
        godot_string device_name_val;
        g_core_api->godot_string_new(&device_name_key);
        g_core_api->godot_string_new(&device_name_val);
        g_core_api->godot_string_parse_utf8(&device_name_key, "name");
        g_core_api->godot_string_parse_utf8(&device_name_val, (devices + i)->name);
        // Device Name - Variant
        godot_variant device_name_var_key;
        godot_variant device_name_var_val;
        g_core_api->godot_variant_new_string(&device_name_var_key, &device_name_key);
        g_core_api->godot_variant_new_string(&device_name_var_val, &device_name_val);
        g_core_api->godot_dictionary_set(&dic, &device_name_var_key, &device_name_var_val);

        // Input Count
        godot_string input_count_key;
        g_core_api->godot_string_new(&input_count_key);
        g_core_api->godot_string_parse_utf8(&input_count_key, "input_count");
        // Input Count - Variant
        godot_variant input_count_var_key;
        godot_variant input_count_var_val;
        g_core_api->godot_variant_new_string(&input_count_var_key, &input_count_key);
        g_core_api->godot_variant_new_int(&input_count_var_val, (devices + i)->input_count);
        g_core_api->godot_dictionary_set(&dic, &input_count_var_key, &input_count_var_val);

        // Output Count
        godot_string output_count_key;
        g_core_api->godot_string_new(&output_count_key);
        g_core_api->godot_string_parse_utf8(&output_count_key, "output_count");
        // Output Count - Variant
        godot_variant output_count_var_key;
        godot_variant output_count_var_val;
        g_core_api->godot_variant_new_string(&output_count_var_key, &output_count_key);
        g_core_api->godot_variant_new_int(&output_count_var_val, (devices + i)->output_count);
        g_core_api->godot_dictionary_set(&dic, &output_count_var_key, &output_count_var_val);
    }
    g_core_api->godot_free(devices);

    godot_variant ret;
    g_core_api->godot_variant_new_dictionary(&ret, &dic);
    return ret;
}
