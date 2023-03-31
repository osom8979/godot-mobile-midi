#ifndef __DEFINED__GMM_H__
#define __DEFINED__GMM_H__

#include <gdnative_api_struct.gen.h>

// GMM == 'G'odot 'M'obile 'M'idi

#define GMM_CLASS_NAME "GodotMobileMidi"
#define GMM_CLASS_BASE "Reference"

#ifndef GODOT_INTERFACE
#define GODOT_INTERFACE(method) gmm_godot_##method
#endif

bool is_android();
bool is_ios();

char const * const get_platform_name();

void set_api(const godot_gdnative_core_api_struct * value);
void set_nativescript_api(const godot_gdnative_ext_nativescript_api_struct * value);
void set_android_api(const godot_gdnative_ext_android_api_struct * value);

const godot_gdnative_core_api_struct * get_api();
const godot_gdnative_ext_nativescript_api_struct * get_nativescript_api();
const godot_gdnative_ext_android_api_struct * get_android_api();

void clear_apis();

// ----------
// Interfaces
// ----------

#define MAX_PLATFORM_LENGTH 16

typedef struct _gmm_data {
    char midi_context_name[256];
    char platform[MAX_PLATFORM_LENGTH];
    void * midi_context;
} gmm_data;

GDCALLINGCONV void * gmm_new(godot_object * obj, void * method);
GDCALLINGCONV void gmm_del(godot_object * obj, void * method, void * user);

godot_variant gmm_get_platform_name(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** args);

#endif // __DEFINED__GMM_H__
