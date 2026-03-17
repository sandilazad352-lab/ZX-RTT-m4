/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 */

#ifndef __ILITEK_H__
#define __ILITEK_H__

#include "drivers/touch.h"
#include <aic_hal_gpio.h>
#include <aic_drv_gpio.h>

#define ILITEK_ADDRESS 0x41

#define ILITEK_I2C_CHAN  QSMD_TOUCH_ILITEK_I2C_CHA
#define ILITEK_RST_PIN   QSMD_TOUCH_ILITEK_RST_PIN
#define ILITEK_INT_PIN   QSMD_TOUCH_ILITEK_INT_PIN

#endif /* __ILITEK_H__ */
