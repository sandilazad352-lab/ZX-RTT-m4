#include <rtthread.h>
#include "zx_gui.h"

static void __zx_gui_event_cb(rt_uint32_t event)
{
    rt_kprintf("event: %u\n", event);
}

static void __zx_gui_mq_cb(const void *buffer, rt_size_t size)
{
    rt_kprintf("mq: %s\n", buffer);
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
    return 0;
}
