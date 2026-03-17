#include <rtthread.h>
#include "zx_gui.h"
#include "zx_ui_entry.h"

static void __zx_gui_event_cb(rt_uint32_t event)
{
}

static void __zx_gui_mq_cb(const void *buffer, rt_size_t size)
{
}

extern void zx_button_adc_start(void);



int __attribute__((weak)) zx_test_entry(void)
{
    return 1;
}

int zx_gui_app_init(void)
{
    if(zx_test_entry())
    {
        zx_gui_set_event_cb(__zx_gui_event_cb);
        zx_gui_set_mq_cb(__zx_gui_mq_cb);
        zx_gui_init();        
    }

    return 0;
}

INIT_APP_EXPORT(zx_gui_app_init);

int main(void)
{
    return 0;
}
