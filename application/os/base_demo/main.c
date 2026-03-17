#include <rtthread.h>
#include "zx_gui.h"

#ifdef AIC_AB_SYSTEM_INTERFACE
#include <absystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <dfs.h>
#include <dfs_fs.h>
#include <boot_param.h>
#endif

static void __zx_gui_event_cb(rt_uint32_t event)
{
}

static void __zx_gui_mq_cb(const void *buffer, rt_size_t size)
{
}

int zx_gui_app_init(void)
{
    zx_gui_set_event_cb(__zx_gui_event_cb);
    zx_gui_set_mq_cb(__zx_gui_mq_cb);
    zx_gui_init();

    return 0;
}

INIT_APP_EXPORT(zx_gui_app_init);

int main(void)
{
#ifdef AIC_AB_SYSTEM_INTERFACE
    char target[32] = { 0 };
    enum boot_device boot_dev = aic_get_boot_device();

    if (boot_dev != BD_SDMC0) {

        aic_ota_status_update();
        aic_get_rodata_to_mount(target);
        printf("Mount APP in blk %s\n", target);

        if (dfs_mount(target, "/rodata", "elm", 0, 0) < 0)
            printf("Failed to mount elm\n");

        memset(target, 0, sizeof(target));

        aic_get_data_to_mount(target);
        printf("Mount APP in blk %s\n", target);

        if (dfs_mount(target, "/data", "elm", 0, 0) < 0)
            printf("Failed to mount elm\n");
    }
#endif
    return 0;
}
