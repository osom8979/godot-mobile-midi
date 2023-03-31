#ifndef __ANDROID__
#error Only Android platform is supported
#endif

#include "gmm.h"
#include <string.h>
#include <jni.h>

typedef struct gmm_platform
{
    JNIEnv * env;
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
    return platform->env != NULL ? true : false;
}
