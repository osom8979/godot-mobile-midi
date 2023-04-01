#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

#ifndef TARGET_OS_IPHONE
#error Only iOS platform is supported
#endif

#include "gmm.h"
#include <string.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/CoreMIDI.h>

typedef struct gmm_platform
{
    int __unknown__;
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
    return true;
}

int gmm_platform_device_count(struct gmm_data * gmm)
{
    return 0;
}
