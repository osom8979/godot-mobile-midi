#ifndef __DEFINED__GMM_H__
#define __DEFINED__GMM_H__

#include <gdnative_api_struct.gen.h>

// GMM == 'G'odot 'M'obile 'M'idi

#define GMM_CLASS_NAME "GodotMobileMidi"
#define GMM_CLASS_BASE "Reference"

#ifndef GODOT_INTERFACE
#define GODOT_INTERFACE(method) gmm_godot_##method
#endif

#define PLATFORM_INDEX_ANDROID  0
#define PLATFORM_INDEX_IOS      1
#define PLATFORM_INDEX_WINDOWS  2
#define PLATFORM_INDEX_MAC      3
#define PLATFORM_INDEX_LINUX    4

bool is_android();
bool is_ios();

char const * const get_platform_name();
int get_platform_index();

void set_core_api(const struct godot_gdnative_core_api_struct * value);
void set_nativescript_api(const struct godot_gdnative_ext_nativescript_api_struct * value);
void set_android_api(const struct godot_gdnative_ext_android_api_struct * value);

const struct godot_gdnative_core_api_struct * get_core_api();
const struct godot_gdnative_ext_nativescript_api_struct * get_nativescript_api();
const struct godot_gdnative_ext_android_api_struct * get_android_api();

void clear_apis();

// ----------
// Interfaces
// ----------

#define MAX_DEVICE_NAME_LENGTH 128
#define MAX_PLATFORM_NAME_LENGTH 16

typedef struct gmm_device_info {
    char device_name[MAX_DEVICE_NAME_LENGTH];
    int input_count;
    int output_count;
} gmm_device_info;

typedef struct gmm_data {
    char platform_name[MAX_PLATFORM_NAME_LENGTH];
    struct gmm_platform * platform;

    godot_variant self;
    godot_string on_read_midi_func_name;

    void (*print)(const char * message);
    bool (*call_on_read_midi)(struct gmm_data * gmm, int v0, int v1);
} gmm_data;

GDCALLINGCONV void * gmm_new(godot_object * obj, void * method);
GDCALLINGCONV void gmm_del(godot_object * obj, void * method, void * user);

godot_variant gmm_get_platform_name(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** args);

godot_variant gmm_init(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** argv);

godot_variant gmm_get_device_count(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** args);

// -------------------
// Platform Interfaces
// -------------------

struct gmm_platform * gmm_platform_alloc();
void gmm_platform_free(struct gmm_platform * platform);
bool gmm_platform_init(struct gmm_platform * platform);

struct gmm_device_info * gmm_platform_devices(struct gmm_data * gmm, unsigned * length);

#endif // __DEFINED__GMM_H__
