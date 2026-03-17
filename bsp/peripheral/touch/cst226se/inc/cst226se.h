/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 */

#ifndef __CST226SE_H__
#define __CST226SE_H__

#include "drivers/touch.h"
#include <aic_hal_gpio.h>
#include <aic_drv_gpio.h>

#define CST226SE_ADDRESS 0x5D

#define CST226SE_I2C_CHAN  QSMD_TOUCH_CST226SE_I2C_CHA
#define CST226SE_RST_PIN   QSMD_TOUCH_CST226SE_RST_PIN
#define CST226SE_INT_PIN   QSMD_TOUCH_CST226SE_INT_PIN

#endif /* __CST226SE_H__ */
