/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 * 2023-04-30     Geo          modified for ZX
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>

#define DBG_TAG "cst226se"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "cst226se.h"

#define CST226SE_MAX_TOUCH 1
#define CST226SE_BUFFER_NUM 28

static struct rt_i2c_client cst226se_client;
static uint16_t __resX;
static uint16_t __resY;

static rt_err_t __cst226se_read_data(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr = dev->client_addr;
    msgs.flags = RT_I2C_RD;
    msgs.buf = data;
    msgs.len = len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

static rt_err_t __cst226se_write_reg(struct rt_i2c_client *dev, rt_uint8_t *data,
                                rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr = dev->client_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

static struct rt_touch_data *read_data;

static void __cst226se_touch_up(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    read_data[id].event = RT_TOUCH_EVENT_UP;

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;
}

static void __cst226se_touch_down(void *buf, int8_t id, int16_t x, int16_t y,
                             int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    read_data[id].event = RT_TOUCH_EVENT_DOWN;
    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;
}

static int __init_cst226se(void)
{           
    uint8_t buffer[8];
    uint8_t cmd[8];
    // Enter Command mode
    cmd[0] = 0xD1;
    cmd[1] = 0x01;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    aic_mdelay(10);
    memset(buffer, 0, sizeof(0));
    cmd[0] = 0xD1;
    cmd[1] = 0xFC;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    memset(buffer, 0, sizeof(0));
    __cst226se_read_data(&cst226se_client, buffer, 4);
    uint32_t checkcode = 0;
    checkcode = buffer[3];
    checkcode <<= 8;
    checkcode |= buffer[2];
    checkcode <<= 8;
    checkcode |= buffer[1];
    checkcode <<= 8;
    checkcode |= buffer[0];
    
    rt_kprintf("Chip checkcode:0x%x.\r\n", checkcode);

    cmd[0] = 0xD1;
    cmd[1] = 0xF8;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    memset(buffer, 0, sizeof(0));
    __cst226se_read_data(&cst226se_client, buffer, 4) ;
    __resX = ( buffer[1] << 8) | buffer[0];
    __resY = ( buffer[3] << 8) | buffer[2];
    rt_kprintf("Chip resolution X:%u Y:%u\r\n", __resX, __resY);
   
    cmd[0] = 0xD2;
    cmd[1] = 0x04;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    memset(buffer, 0, sizeof(0));
    __cst226se_read_data(&cst226se_client, buffer, 4);
    uint32_t chipType = buffer[3];
    chipType <<= 8;
    chipType |= buffer[2];

    
    uint32_t ProjectID = buffer[1];
    ProjectID <<= 8;
    ProjectID |= buffer[0];
    
    rt_kprintf("Chip type :0x%X, ProjectID:0X%x\r\n",
          chipType, ProjectID);


    cmd[0] = 0xD2;
    cmd[1] = 0x08;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    memset(buffer, 0, sizeof(0));
    __cst226se_read_data(&cst226se_client, buffer, 4);
    
    uint32_t fwVersion = buffer[3];
    fwVersion <<= 8;
    fwVersion |= buffer[2];
    fwVersion <<= 8;
    fwVersion |= buffer[1];
    fwVersion <<= 8;
    fwVersion |= buffer[0];
    
    uint32_t checksum = buffer[7];
    checksum <<= 8;
    checksum |= buffer[6];
    checksum <<= 8;
    checksum |= buffer[5];
    checksum <<= 8;
    checksum |= buffer[4];
    
    rt_kprintf("Chip ic version:0x%X, checksum:0x%X",
          fwVersion, checksum);
    
    if (fwVersion == 0xA5A5A5A5) {
        rt_kprintf("Chip ic don't have firmware. ");
        return -1;
    }
    if ((checkcode & 0xffff0000) != 0xCACA0000) {
        rt_kprintf("Firmware info read error .");
        return -1;
    }
    
    cmd[0] = 0xD2;
    cmd[1] = 0x09;
    __cst226se_write_reg(&cst226se_client, cmd, 2);
    return 1;
}

static rt_size_t cst226se_read_point(struct rt_touch_device *touch, void *buf,
                                  rt_size_t read_num)
{
    uint8_t buffer[CST226SE_BUFFER_NUM];
    uint8_t cmd[2];
    uint8_t index = 0;
    uint16_t input_x = 0, input_y = 0;
    uint16_t tmp_x, tmp_y;
    uint8_t status;
    read_num = 0;

    memset(cmd, 0, sizeof(cmd));
    cmd[0] = 0x00;
    __cst226se_write_reg(&cst226se_client, cmd, 1);
    memset(buffer, 0, sizeof(buffer));
    if (__cst226se_read_data(&cst226se_client, buffer, CST226SE_BUFFER_NUM) != RT_EOK) {
        LOG_I("read point failed\n");
        read_num = 0;
        goto __exit;
    }

    if (buffer[6] != 0xAB)
        goto __exit;
    if (buffer[0] == 0xAB)
        goto __exit;
    if (buffer[5] == 0x80)
        goto __exit;

    uint8_t point = buffer[5] & 0x7F;
    if (point > 5  || !point) {
        memset(cmd, 0, sizeof(cmd));
        cmd[0] = 0x00;
        cmd[1] = 0xAB;
        __cst226se_write_reg(&cst226se_client, cmd, 2);
        goto __exit;
    }

    status = buffer[index] & 0x0F;
    tmp_x = (uint16_t)((buffer[index + 1] << 4) | ((buffer[index + 3] >> 4) & 0x0F));
    tmp_y = (uint16_t)((buffer[index + 2] << 4) | (buffer[index + 3] & 0x0F));
    input_x = __resY - tmp_y;
    input_y = tmp_x;
    read_num = 1;

    LOG_D("%u/%u/%u/%u\n", input_x, input_y, point, status);
    if (status == 0)
        __cst226se_touch_up(buf, 0, input_x, input_y, 0);
    else
        __cst226se_touch_down(buf, 0, input_x, input_y, 0);

__exit:

    return read_num;
}

static rt_err_t __cst226se_get_info(struct rt_i2c_client *dev,
                               struct rt_touch_info *info)
{
    info->range_x = __resX;
    info->range_y = __resY;
    info->point_num = 1;

    return RT_EOK;
}

static rt_err_t cst226se_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    if (cmd == RT_TOUCH_CTRL_GET_INFO) {
        return __cst226se_get_info(&cst226se_client, arg);
    }

    return -RT_ERROR;
}

static struct rt_touch_ops cst226se_touch_ops = {
    .touch_readpoint = cst226se_read_point,
    .touch_control = cst226se_control,
};

static int rt_hw_cst226se_init(const char *name, struct rt_touch_config *cfg)
{
    struct rt_touch_device *touch_device = RT_NULL;

    touch_device =
        (struct rt_touch_device *)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL) {
        LOG_E("touch device malloc fail");
        return -RT_ERROR;
    }
    rt_memset((void *)touch_device, 0, sizeof(struct rt_touch_device));

    /* hw init*/
    // rst output 0
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_LOW);
    rt_thread_delay(10);

    // irq output 0
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(cfg->irq_pin.pin, PIN_LOW);

    rt_thread_delay(2);
    // rst output 1
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_HIGH);

    rt_thread_delay(5);
    // rst input
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_INPUT);

    //irq output 0
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(cfg->irq_pin.pin, PIN_LOW);

    rt_thread_delay(50);

    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_INPUT_PULLUP);

    cst226se_client.bus =
        (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (cst226se_client.bus == RT_NULL) {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)cst226se_client.bus, RT_DEVICE_FLAG_RDWR) !=
        RT_EOK) {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    cst226se_client.client_addr = CST226SE_ADDRESS;

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &cst226se_touch_ops;

    if (RT_EOK != rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL)) {
        LOG_E("touch device cst226se init failed !!!");
        return -RT_ERROR;
    }

    __init_cst226se();
    LOG_I("touch device cst226se init success");
    return RT_EOK;
}

static int rt_cst226se_gpio_cfg()
{
    unsigned int g, p;
    long pin;

    // RST
    pin = drv_pin_get(CST226SE_RST_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);

    // INT
    pin = drv_pin_get(CST226SE_INT_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);
    hal_gpio_set_irq_mode(g, p, 0);

    return 0;
}

static int rt_hw_cst226se_port(void)
{
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rt_cst226se_gpio_cfg();

    rst_pin = drv_pin_get(CST226SE_RST_PIN);
    cfg.dev_name = CST226SE_I2C_CHAN;
    cfg.irq_pin.pin = drv_pin_get(CST226SE_INT_PIN);
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.user_data = &rst_pin;

    rt_hw_cst226se_init("cst226se", &cfg);

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_cst226se_port);
