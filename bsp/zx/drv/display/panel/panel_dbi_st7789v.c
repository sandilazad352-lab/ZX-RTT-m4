/*
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "panel_dbi.h"

/* Init sequence, each line consists of command, count of data, data... */
static const u8 st7789v_commands[] = {
    0x00, 1, 120,
    0x11, 0,
    0x00, 1, 120,
    0x36, 1, 0x00,   
    0x3A, 1, 0x66,  // 0x55 -> rgb565   0x66 -> rgb666
    0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33,
    0xB7, 1, 0x75,   //VGH=14.97V,VGL=-7.67V
    0xBB, 1, 0x1C,
    0xC0, 1, 0x2C,
    0xC2, 1, 0x01,
    0xC3, 1, 0x13,
    0xC4, 1, 0x20,
    0xC6, 1, 0x0F,
    0xD0, 2, 0xA4, 0xA1,   
    0xD6, 1, 0xA1, 
    0xE0, 14, 0xD0, 0x0C, 0x12, 0x0E, 0x0D, 0x08, 0x36, 0x44, 0x4B, 0x39, 0x15, 0x15, 0x2C, 0x2F, 
    0xE1, 14, 0xD0, 0x09, 0x0E, 0x08, 0x07, 0x14, 0x35, 0x43, 0x4B, 0x36, 0x12, 0x13, 0x29, 0x2D,
    0x21, 0,
    0x29, 0,
    0x2C, 0,
};

static struct aic_panel_funcs st7789v_funcs = {
    .prepare = panel_default_prepare,
    .enable = panel_dbi_default_enable,
    .disable = panel_default_disable,
    .unprepare = panel_default_unprepare,
    .register_callback = panel_register_callback,
};

#if 0
// benchmark 67
static struct display_timing st7789v_timing = {
    .pixelclock   = 6000000,

    .hactive      = 240,
    .hback_porch  = 10,
    .hfront_porch = 38,
    .hsync_len    = 10,

    .vactive      = 320,
    .vback_porch  = 4,
    .vfront_porch = 8,
    .vsync_len    = 4,
};
#elif 1
// benchmark 70
static struct display_timing st7789v_timing = {
    .pixelclock   = 5000000,

    .hactive      = 240,
    .hback_porch  = 2,
    .hfront_porch = 3,
    .hsync_len    = 1,

    .vactive      = 320,
    .vback_porch  = 3,
    .vfront_porch = 2,
    .vsync_len    = 1,
};
#else
// benchmark 84
static struct display_timing st7789v_timing = {
    .pixelclock   = 6000000,

    .hactive      = 240,
    .hback_porch  = 2,
    .hfront_porch = 3,
    .hsync_len    = 1,

    .vactive      = 320,
    .vback_porch  = 3,
    .vfront_porch = 2,
    .vsync_len    = 1,
};
#endif

static struct panel_dbi dbi = {
    .type = I8080,
    .format = I8080_RGB666_16BIT_3CYCLE,
    .commands = {
        .buf = st7789v_commands,
        .len = ARRAY_SIZE(st7789v_commands),
    }
};

struct aic_panel dbi_st7789v = {
    .name = "panel-st7789v",
    .timings = &st7789v_timing,
    .funcs = &st7789v_funcs,
    .dbi = &dbi,
    .connector_type = AIC_DBI_COM,
};
