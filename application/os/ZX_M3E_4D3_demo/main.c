#include <rtthread.h>
#include "zx_gui.h"
#include "zx_ui_entry.h"
#include "ui_common.h"
#include <stdio.h>
#include <stdlib.h>
static void __zx_gui_event_cb(rt_uint32_t event)
{
    
}

static void __zx_gui_mq_cb(const void *buffer, rt_size_t size)
{
    char *buf = (char *)buffer;
    if(strstr(buf, "key") != NULL)
    {
        char *key = strtok(buf, "-");
        if(key != NULL)
        {
            BTN_STATUS_t btn_status = atoi(strtok(NULL, "-"));

            Event_Data_t xReceivedEvent;
            xReceivedEvent.eEventID = E_KEY_EVENT;
            if(0 == strcmp(key, "key1"))
                xReceivedEvent.lDataArray[0] = E_K1;
            else if(0 == strcmp(key, "key2"))
                xReceivedEvent.lDataArray[0] = E_K2;
            else if(0 == strcmp(key, "key3"))
                xReceivedEvent.lDataArray[0] = E_K3;
            else if(0 == strcmp(key, "key4"))
                xReceivedEvent.lDataArray[0] = E_K4;
            
            xReceivedEvent.lDataArray[1] = btn_status;


            scr_refr_func(&xReceivedEvent); // 处理各类事件
        }
        
    }
}

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
