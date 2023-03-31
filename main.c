#include <assert.h>

#include "gmm.h"

void GDN_EXPORT GODOT_INTERFACE(gdnative_init)(godot_gdnative_init_options * options)
{
    const godot_gdnative_core_api_struct * api = options->api_struct;
    assert(api != NULL);

    set_api(api);

    for (unsigned i = 0; i < api->num_extensions; ++i) {
        switch (api->extensions[i]->type) {
            case GDNATIVE_EXT_NATIVESCRIPT:
                set_nativescript_api((godot_gdnative_ext_nativescript_api_struct*)api->extensions[i]);
                break;
            case GDNATIVE_EXT_ANDROID:
                set_android_api((godot_gdnative_ext_android_api_struct*)api->extensions[i]);
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
    const godot_gdnative_ext_nativescript_api_struct * nativescript = get_nativescript_api();
    assert(handle != NULL);
    assert(nativescript != NULL);

    godot_instance_create_func create = {NULL, NULL, NULL};
    create.create_func = &gmm_new;

    godot_instance_destroy_func destroy = {NULL, NULL, NULL};
    destroy.destroy_func = &gmm_del;

    nativescript->godot_nativescript_register_class(
            handle,
            GMM_CLASS_NAME,
            GMM_CLASS_BASE,
            create,
            destroy);
}

void _register_get_platform_name(void * handle)
{
    const godot_gdnative_ext_nativescript_api_struct * nativescript = get_nativescript_api();
    assert(handle != NULL);
    assert(nativescript != NULL);

    godot_instance_method get_platform_name = {NULL, NULL, NULL};
    get_platform_name.method = &gmm_get_platform_name;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    nativescript->godot_nativescript_register_method(
            handle,
            GMM_CLASS_NAME,
            "get_platform_name",
            attributes,
            get_platform_name);
}

void _register_init(void * handle)
{
    const godot_gdnative_ext_nativescript_api_struct * nativescript = get_nativescript_api();
    assert(handle != NULL);
    assert(nativescript != NULL);

    godot_instance_method init = {NULL, NULL, NULL};
    init.method = &gmm_init;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};
    nativescript->godot_nativescript_register_method(
            handle,
            GMM_CLASS_NAME,
            "init",
            attributes,
            init);
}

void GDN_EXPORT GODOT_INTERFACE(nativescript_init)(void * handle)
{
    _register_create_and_destroy(handle);
    _register_get_platform_name(handle);
    _register_init(handle);
}
