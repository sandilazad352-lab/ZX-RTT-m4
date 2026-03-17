#include <rtthread.h>
#include "zx_gui.h"
#include "zx_ui_entry.h"

static lv_obj_t *encode_label = NULL, *gxhtc3c_label = NULL;

extern volatile int32_t encoder_count;
extern int humidity, temp;

static void count_refr(lv_timer_t *t)
{
    static int _tmp_data = 0;

    if(_tmp_data != encoder_count / 4)
    {
        _tmp_data = encoder_count / 4;
        lv_label_set_text_fmt(encode_label, "%d", _tmp_data);
    }
}
extern void gxhtc3c(int argc, char *argv[]);
static void gxhtc3c_refr(lv_timer_t *t)
{
    char *cmd[] = {
        "gxhtc3c",
        "read"
    };
    gxhtc3c(2, cmd);
    lv_label_set_text_fmt(gxhtc3c_label, "temp: %d.%d, humidity: %d.%d", temp / 10, temp % 10, humidity / 10, humidity % 10);
}

static void encoder_label_create(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(255, 0, 0), 0);
    
    encode_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(encode_label, &lv_font_montserrat_40, NULL);
    lv_obj_set_style_text_color(encode_label, lv_color_white(), NULL);
    lv_label_set_text(encode_label, "0");
    lv_obj_center(encode_label);

    gxhtc3c_label = lv_label_create(lv_scr_act());
    lv_label_set_text(gxhtc3c_label, "temp: 0, humidity: 0");
    lv_obj_set_style_text_color(gxhtc3c_label, lv_color_white(), NULL);
    lv_obj_align(gxhtc3c_label, LV_ALIGN_TOP_RIGHT, -10, 10);

    lv_timer_create(count_refr, 30, NULL);
    lv_timer_create(gxhtc3c_refr, 2000, NULL);

    extern int test_encoder(int argc, char *argv[]);
    test_encoder(0, NULL);

    char *cmd[] = {
        "gxhtc3c",
        "probe",
        "i2c1",
    };
    gxhtc3c(3, cmd);

}

static void __zx_gui_event_cb(rt_uint32_t event)
{
    rt_uint16_t key;
    rt_uint16_t ctrl;
    key = event >> 8;
    ctrl = event & 0xff;
    static int key_lock = 0;
    static int color_chg = 0;
    lv_color_t color_list[] = {
        LV_COLOR_MAKE(255, 0, 0),
        LV_COLOR_MAKE(0, 255, 0),
        LV_COLOR_MAKE(0, 0, 255),
        LV_COLOR_MAKE(255, 255, 255),
        LV_COLOR_MAKE(0, 0, 0),
    };

    if(key == 0x1)
    {
        lv_obj_set_style_bg_color(lv_scr_act(), color_list[color_chg], NULL);
        color_chg = (color_chg + 1) % 5;
    }

    if(key_lock)
        return;

    else if(key == 0x2)
    {
        lv_obj_clean(lv_scr_act());
        encoder_label_create();
        key_lock = 1;
    }
    else if(key == 0x4)
    {
        lv_obj_clean(lv_scr_act());
        lv_demo_benchmark();
        key_lock = 1;
    }
}

static void __zx_gui_mq_cb(const void *buffer, rt_size_t size)
{
}

extern void zx_button_adc_start(void);

int zx_gui_app_init(void)
{
    zx_gui_set_event_cb(__zx_gui_event_cb);
    zx_gui_set_mq_cb(__zx_gui_mq_cb);
    zx_gui_init();
#ifdef LPKG_USING_ZX_BUTTON
    zx_button_start_gpio();
#endif

    return 0;
}

INIT_APP_EXPORT(zx_gui_app_init);

int main(void)
{
    return 0;
}
