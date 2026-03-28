#include "zx_ui_entry.h"
#include "../services/zx_mqtt_publish.h"
#include "../services/zx_mqtt_subscribe.h"

#include <rtthread.h>
#include <string.h>

static lv_obj_t *s_textarea;
static lv_obj_t *s_keyboard;
static lv_obj_t *s_sub_msg_label;

static char s_sub_msg[128] = "Sub message: waiting...";
static rt_bool_t s_sub_msg_dirty = RT_TRUE;
static rt_mutex_t s_sub_msg_lock = RT_NULL;

static void sub_msg_timer_cb(lv_timer_t *timer)
{
    LV_UNUSED(timer);

    if (!s_sub_msg_dirty || s_sub_msg_label == NULL || s_sub_msg_lock == RT_NULL)
    {
        return;
    }

    rt_mutex_take(s_sub_msg_lock, RT_WAITING_FOREVER);
    lv_label_set_text(s_sub_msg_label, s_sub_msg);
    s_sub_msg_dirty = RT_FALSE;
    rt_mutex_release(s_sub_msg_lock);
}

static void mqtt_sub_msg_cb(const char *topic,
                            size_t topic_len,
                            const char *payload,
                            size_t payload_len)
{
    if (payload == RT_NULL || payload_len == 0 || s_sub_msg_lock == RT_NULL)
    {
        return;
    }

    rt_mutex_take(s_sub_msg_lock, RT_WAITING_FOREVER);

    rt_snprintf(s_sub_msg, sizeof(s_sub_msg), "Sub %.*s: %.*s",
                (int)topic_len, topic,
                (int)((payload_len > 80) ? 80 : payload_len), payload);
    s_sub_msg_dirty = RT_TRUE;

    rt_mutex_release(s_sub_msg_lock);
}

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

        if (zx_mqtt_publish_text(text) < 0)
        {
            return;
        }

        lv_textarea_set_text(s_textarea, "");
        lv_keyboard_set_textarea(s_keyboard, NULL);
        lv_obj_add_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

void zx_ui_entry(void)
{
    lv_obj_t *scr = lv_scr_act();

    if (s_sub_msg_lock == RT_NULL)
    {
        s_sub_msg_lock = rt_mutex_create("submsg", RT_IPC_FLAG_FIFO);
    }

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

    s_sub_msg_label = lv_label_create(scr);
    lv_obj_set_width(s_sub_msg_label, 300);
    lv_obj_align_to(s_sub_msg_label, send_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 14);
    lv_label_set_long_mode(s_sub_msg_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(s_sub_msg_label, s_sub_msg);

    s_keyboard = lv_keyboard_create(scr);
    lv_obj_set_size(s_keyboard, lv_pct(100), 120);
    lv_obj_align(s_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(s_keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(s_keyboard, keyboard_event_cb, LV_EVENT_ALL, NULL);

    lv_timer_create(sub_msg_timer_cb, 100, NULL);

    zx_mqtt_set_subscribe_callback(mqtt_sub_msg_cb);
    if (zx_mqtt_subscribe_default_topic() < 0)
    {
        rt_snprintf(s_sub_msg, sizeof(s_sub_msg), "Sub message: subscribe failed");
        s_sub_msg_dirty = RT_TRUE;
    }
}