#include <gdnative_api_struct.gen.h>

#include <assert.h>
#include <string.h>

const godot_gdnative_core_api_struct * g_api = NULL;
const godot_gdnative_ext_nativescript_api_struct * g_nativescript_api = NULL;

// GMM == 'G'odot 'M'obile 'M'idi

typedef struct _gmm_data {
    char data[256];
} gmm_data;

GDCALLINGCONV void * gmm_new(godot_object * obj, void * method)
{
    assert(g_api != NULL);

    gmm_data * result = g_api->godot_alloc(sizeof(gmm_data));
    strcpy(result->data, "Hello, World!");
    return result;
}

GDCALLINGCONV void gmm_del(godot_object * obj, void * method, void * user)
{
    assert(g_api != NULL);
    assert(user != NULL);

    g_api->godot_free(user);
}

godot_variant gmm_get_data(
        godot_object * obj,
        void * method,
        void * user,
        int argc,
        godot_variant ** args)
{
    assert(g_api != NULL);

    gmm_data * gmm_user = (gmm_data*)user;

    godot_string text;
    g_api->godot_string_new(&text);
    g_api->godot_string_parse_utf8(&text, gmm_user->data);

    godot_variant result;
    g_api->godot_variant_new_string(&result, &text);

    g_api->godot_string_destroy(&text);

    return result;
}

// -------------------
// Godot Native Export
// -------------------

void GDN_EXPORT gmm_godot_gdnative_init(godot_gdnative_init_options * options)
{
    g_api = options->api_struct;
    assert(g_api != NULL);

    for (unsigned i = 0; i < g_api->num_extensions; ++i) {
        switch (g_api->extensions[i]->type) {
            case GDNATIVE_EXT_NATIVESCRIPT:
                g_nativescript_api = (godot_gdnative_ext_nativescript_api_struct*)g_api->extensions[i];
                break;
            default:
                break;
        }
    }
}

void GDN_EXPORT gmm_godot_gdnative_terminate(godot_gdnative_terminate_options * options)
{
    g_api = NULL;
    g_nativescript_api = NULL;
}

void GDN_EXPORT gmm_godot_nativescript_init(void * handle)
{
    godot_instance_create_func create = {NULL, NULL, NULL};
    create.create_func = &gmm_new;

    godot_instance_destroy_func destroy = {NULL, NULL, NULL};
    destroy.destroy_func = &gmm_del;

    g_nativescript_api->godot_nativescript_register_class(handle, "GodotMobileMidi", "Reference", create, destroy);

    godot_instance_method get_data = {NULL, NULL, NULL};
    get_data.method = &gmm_get_data;

    godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};

    g_nativescript_api->godot_nativescript_register_method(handle, "GodotMobileMidi", "get_data", attributes, get_data);
}

