#ifndef __CST3240_H__
#define __CST3240_H__

#include "drivers/touch.h"
#include "stdatomic.h"
#include <aic_hal_gpio.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>
#include <aic_drv_gpio.h>

#define CST3240_ADDRESS 0x5A

#define TOUCH_EVT_PRESS     0x06    //按下
#define TOUCH_EVT_RELEASE   0x00    //抬起

#define CST3240_I2C_CHAN  QSMD_TOUCH_CST3240_I2C_CHA
#define CST3240_RST_PIN   QSMD_TOUCH_CST3240_RST_PIN
#define CST3240_INT_PIN   QSMD_TOUCH_CST3240_INT_PIN

#endif /* __CST3240_H__ */
