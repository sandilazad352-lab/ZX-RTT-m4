#include "zx_ui_entry.h"
#include <rtthread.h>
#include <string.h>

#ifdef PKG_USING_UMQTT
#include "umqtt.h"
#endif

#define CUSTDEMO_MQTT_URI       "tcp://172.16.16.4:1883"
#define CUSTDEMO_MQTT_TOPIC     "/mqtt/test"
#define CUSTDEMO_MQTT_CLIENT_ID "custdemo_ui"

static lv_obj_t *s_textarea;
static lv_obj_t *s_keyboard;

#ifdef PKG_USING_UMQTT
static umqtt_client_t s_mqtt_client;
static rt_bool_t s_mqtt_started = RT_FALSE;

static int mqtt_start_if_needed(void)
{
    if (s_mqtt_started && s_mqtt_client != RT_NULL)
    {
        return 0;
    }

    struct umqtt_info info = {0};
    info.uri = CUSTDEMO_MQTT_URI;
    info.client_id = CUSTDEMO_MQTT_CLIENT_ID;

    s_mqtt_client = umqtt_create(&info);
    if (s_mqtt_client == RT_NULL)
    {
        rt_kprintf("[custdemo] mqtt create failed\n");
        return -1;
    }

    if (umqtt_start(s_mqtt_client) < 0)
    {
        rt_kprintf("[custdemo] mqtt start failed\n");
        umqtt_delete(s_mqtt_client);
        s_mqtt_client = RT_NULL;
        return -1;
    }

    s_mqtt_started = RT_TRUE;
    rt_kprintf("[custdemo] mqtt connected: %s\n", CUSTDEMO_MQTT_URI);
    return 0;
}
#endif

static void keyboard_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_keyboard_set_textarea(s_keyboard, NULL);
        lv_obj_add_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

static void textarea_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_FOCUSED)
    {
        lv_keyboard_set_textarea(s_keyboard, s_textarea);
        lv_obj_clear_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

static void send_btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        const char *text = lv_textarea_get_text(s_textarea);

        if (text == RT_NULL || text[0] == '\0')
        {
            rt_kprintf("[custdemo] empty text, skip publish\n");
            return;
        }

#ifdef PKG_USING_UMQTT
        if (mqtt_start_if_needed() < 0)
        {
            rt_kprintf("[custdemo] mqtt not ready\n");
            return;
        }

        int ret = umqtt_publish(s_mqtt_client,
                                UMQTT_QOS1,
                                CUSTDEMO_MQTT_TOPIC,
                                (void *)text,
                                strlen(text),
                                1000);
        if (ret < 0)
        {
            rt_kprintf("[custdemo] mqtt publish failed: %d\n", ret);
            return;
        }

        rt_kprintf("[custdemo] mqtt publish ok topic=%s payload=%s\n", CUSTDEMO_MQTT_TOPIC, text);
#else
        rt_kprintf("[custdemo] PKG_USING_UMQTT not enabled\n");
        return;
#endif

        lv_textarea_set_text(s_textarea, "");
        lv_keyboard_set_textarea(s_keyboard, NULL);
        lv_obj_add_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void zx_ui_entry(void)
{
    lv_obj_t *scr = lv_scr_act();

    s_textarea = lv_textarea_create(scr);
    lv_obj_set_width(s_textarea, 280);
    lv_obj_align(s_textarea, LV_ALIGN_TOP_MID, 0, 20);
    lv_textarea_set_one_line(s_textarea, true);
    lv_textarea_set_placeholder_text(s_textarea, "Type message...");
    lv_obj_add_event_cb(s_textarea, textarea_event_cb, LV_EVENT_FOCUSED, NULL);

    lv_obj_t *send_btn = lv_btn_create(scr);
    lv_obj_set_size(send_btn, 120, 42);
    lv_obj_align_to(send_btn, s_textarea, LV_ALIGN_OUT_BOTTOM_MID, 0, 16);
    lv_obj_add_event_cb(send_btn, send_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_label = lv_label_create(send_btn);
    lv_label_set_text(btn_label, "Send");
    lv_obj_center(btn_label);

    s_keyboard = lv_keyboard_create(scr);
    lv_obj_set_size(s_keyboard, lv_pct(100), 120);
    lv_obj_align(s_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(s_keyboard, keyboard_event_cb, LV_EVENT_ALL, NULL);
}