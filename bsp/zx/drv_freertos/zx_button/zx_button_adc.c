#include "zx_button.h"

typedef struct {
    uint8_t is_configured;
    struct {
        uint8_t is_init;
        uint16_t volt_mv;
    } channel[GPAI_NUM_MAX];
} adc_data_t;

typedef struct {
    uint8_t channel;
    uint16_t max_volt_mv;
    uint16_t min_volt_mv;
} btn_hw_info_t;

static adc_data_t g_btn = {0};

uint8_t zx_button_adc_read(uint8_t scan_start, uint8_t wait_press, void* hardware_data) {
    if (scan_start) {
        for (uint8_t i = 0; i < GPAI_NUM_MAX; i++) {
            if (g_btn.channel[i].is_init) {
                g_btn.channel[i].volt_mv = gpai_read_mv(i);
            }
        }
    }
    btn_hw_info_t* hw_info = hardware_data;
    return g_btn.channel[hw_info->channel].volt_mv > hw_info->max_volt_mv || g_btn.channel[hw_info->channel].volt_mv < hw_info->min_volt_mv;
}

btn_handle_t zx_button_create_adc(uint32_t adc_channel, uint16_t middle_volt_mv, uint16_t diff_volt_mv, void* user_data) {
    if (g_btn.is_configured == 0) {
        gpai_init();
        g_btn.is_configured = 1;
    }

    if (g_btn.channel[adc_channel].is_init == 0) {
        gpai_enabled(adc_channel, 1, AIC_GPAI_MODE_PERIOD, 100);
        g_btn.channel[adc_channel].is_init = 1;
    }
    btn_hw_info_t* hw_info = (btn_hw_info_t *)malloc(sizeof(btn_hw_info_t));
    hw_info->channel = adc_channel;
    hw_info->max_volt_mv = middle_volt_mv + diff_volt_mv;
    if (diff_volt_mv > middle_volt_mv) {
        hw_info->min_volt_mv = 0;    
    } else {
        hw_info->min_volt_mv = middle_volt_mv - diff_volt_mv;
    }

    return zx_button_create(zx_button_adc_read, (void *)hw_info, 0, user_data);
}
