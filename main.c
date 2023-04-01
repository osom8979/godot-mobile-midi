#include <assert.h>

#include "gmm.h"

void GDN_EXPORT GODOT_INTERFACE(gdnative_init)(godot_gdnative_init_options * options)
{
    const godot_gdnative_core_api_struct * core = options->api_struct;
    assert(core != NULL);

    set_core_api(core);

    for (unsigned i = 0; i < core->num_extensions; ++i) {
        switch (core->extensions[i]->type) {
            case GDNATIVE_EXT_NATIVESCRIPT:
                set_nativescript_api((struct godot_gdnative_ext_nativescript_api_struct*)core->extensions[i]);
                break;
            case GDNATIVE_EXT_ANDROID:
                set_android_api((struct godot_gdnative_ext_android_api_struct*)core->extensions[i]);
                break;
            default:
                break;
        }
    }
}

void GDN_EXPORT GODOT_INTERFACE(gdnative_terminate)(godot_gdnative_terminate_options * options)
{
    clear_apis();
}

void _register_create_and_destroy(void * handle)
{
    godot_instance_create_func create_method = {NULL, NULL, NULL};
    create_method.create_func = &gmm_new;

    godot_instance_destroy_func destroy_method = {NULL, NULL, NULL};
    destroy_method.destroy_func = &gmm_del;

    get_nativescript_api()->godot_nativescript_register_class(
            handle,
            GMM_CLASS_NAME,
            GMM_CLASS_BASE,
            create_method,
            destroy_method);
}

void _register_get_platform_name(void * handle)
{
    godot_instance_method method = {NULL, NULL, NULL};
    method.method = &gmm_get_platform_name;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    get_nativescript_api()->godot_nativescript_register_method(
            handle,
            GMM_CLASS_NAME,
            "get_platform_name",
            attributes,
            method);
}

void _register_init(void * handle)
{
    godot_instance_method method = {NULL, NULL, NULL};
    method.method = &gmm_init;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    get_nativescript_api()->godot_nativescript_register_method(
            handle,
            GMM_CLASS_NAME,
            "init",
            attributes,
            method);
}

void _register_get_device_count(void * handle)
{
    godot_instance_method method = {NULL, NULL, NULL};
    method.method = &gmm_get_device_count;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    get_nativescript_api()->godot_nativescript_register_method(
            handle,
            GMM_CLASS_NAME,
            "get_device_count",
            attributes,
            method);
}

void GDN_EXPORT GODOT_INTERFACE(nativescript_init)(void * handle)
{
    assert(handle != NULL);

    _register_create_and_destroy(handle);
    _register_get_platform_name(handle);
    _register_init(handle);
    _register_get_device_count(handle);
}
