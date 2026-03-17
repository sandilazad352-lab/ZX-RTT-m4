#include "zx_button.h"
#include <drivers/adc.h>
#include "aic_core.h"
#include "aic_hal_clk.h"
#include "hal_gpai.h"

#define TAG "BTN_ADC"

#define AIC_GPAI_NAME      "gpai"
#define MAX_ADC_CHANNEL 8

struct rt_adc_device *gpai_dev = NULL;

typedef struct {
    uint8_t is_configured;
    struct {
        uint8_t is_init;
        uint16_t volt_mv;
    } channel[MAX_ADC_CHANNEL];
} adc_data_t;

typedef struct {
    uint8_t channel;
    uint16_t max_volt_mv;
    uint16_t min_volt_mv;
} btn_hw_info_t;

static adc_data_t g_btn = {0};

uint16_t get_volt_mv(int channel) {
    return rt_adc_read(gpai_dev, channel);
}


uint8_t zx_button_adc_read(uint8_t scan_start, uint8_t wait_press, void* hardware_data) {
    if (scan_start) {
        for (uint8_t i = 0; i < MAX_ADC_CHANNEL; i++) {
            if (g_btn.channel[i].is_init) {
                g_btn.channel[i].volt_mv = get_volt_mv(i);
                // rt_kprintf("volt_mv[%d]:%u \n", i, g_btn.channel[i].volt_mv);
            }
        }
    }
    btn_hw_info_t* hw_info = hardware_data;
    return g_btn.channel[hw_info->channel].volt_mv > hw_info->max_volt_mv || g_btn.channel[hw_info->channel].volt_mv < hw_info->min_volt_mv;
}


btn_handle_t zx_button_create_adc(int adc_channel, uint16_t middle_volt_mv, uint16_t diff_volt_mv,  uint8_t active_level, void* user_data) {
    if (!gpai_dev) {
        gpai_dev = (struct rt_adc_device *)rt_device_find(AIC_GPAI_NAME);
        if (!gpai_dev) {
            rt_kprintf("Failed to open %s device\n", AIC_GPAI_NAME);
            return NULL;
        }
    }

    if (g_btn.channel[adc_channel].is_init == 0) {
        struct aic_gpai_ch *chan = NULL;

        chan = hal_gpai_ch_is_valid(adc_channel);
        if (!chan)
        {
            rt_kprintf("Ch%d is unavailable!\n", chan);
            return NULL;
        }

        rt_adc_enable(gpai_dev, adc_channel);
        rt_thread_delay(2);
        g_btn.channel[adc_channel].is_init = 1;
    }
    // else{
    //     return NULL;
    // }

    btn_hw_info_t* hw_info = (btn_hw_info_t *)malloc(sizeof(btn_hw_info_t));
    hw_info->channel = adc_channel;
    hw_info->max_volt_mv = middle_volt_mv + diff_volt_mv;
    if (diff_volt_mv > middle_volt_mv) {
        hw_info->min_volt_mv = 0;    
    } else {
        hw_info->min_volt_mv = middle_volt_mv - diff_volt_mv;
    }
    
    return zx_button_create(zx_button_adc_read, (void *)hw_info, active_level, user_data);
}
