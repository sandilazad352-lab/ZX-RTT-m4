/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "rtdevice.h"
#include "board.h"
#include <string.h>
#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/
/* software simulation serial transmit IRQ handler thread stack */
#ifdef rt_align
rt_align(RT_ALIGN_SIZE)
#else
ALIGN(RT_ALIGN_SIZE)
#endif
static rt_uint8_t serial_soft_trans_irq_stack[1024*2];
/* software simulation serial transmit IRQ handler thread */
static struct rt_thread thread_serial_soft_trans_irq;
/* serial event */
static struct rt_event event_serial;
/* modbus master serial device */
// static struct rt_serial_device *serial;
static rt_device_t serial_dev = RT_NULL;
/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);
static rt_err_t serial_rx_ind(rt_device_t dev, rt_size_t size);
static void serial_soft_trans_irq(void* parameter);

static struct rt_serial_device *serial;
static struct serial_configure uart_conf = RT_SERIAL_CONFIG_DEFAULT;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{

    char uart_name[20];

    /**
     * set 485 mode receive and transmit control IO
     * @note MODBUS_MASTER_RT_CONTROL_PIN_INDEX need be defined by user
     */
#if defined(RT_MODBUS_MASTER_USE_CONTROL_PIN)
    rt_pin_mode(MODBUS_MASTER_RT_CONTROL_PIN_INDEX, PIN_MODE_OUTPUT);
#endif
    /* set serial name */
    rt_snprintf(uart_name,sizeof(uart_name), "uart%d", ucPORT);

    serial_dev = rt_device_find(uart_name);
    if(serial_dev == RT_NULL)
    {
        /* can not find uart */
        rt_kprintf("can not find %s\n", uart_name);
        return FALSE;
    }
    rt_kprintf("rt_device_find:%s\n", uart_name);

    serial = (struct rt_serial_device*)serial_dev;

    rt_memcpy(&uart_conf, &(serial->config), sizeof(uart_conf));
    /* set serial configure parameter */
    uart_conf.baud_rate = ulBaudRate;
    uart_conf.stop_bits = STOP_BITS_1;
    switch(eParity){
    case MB_PAR_NONE: {
        uart_conf.data_bits = DATA_BITS_8;
        uart_conf.parity = PARITY_NONE;
        break;
    }
    case MB_PAR_ODD: {
        uart_conf.data_bits = DATA_BITS_9;
        uart_conf.parity = PARITY_ODD;
        break;
    }
    case MB_PAR_EVEN: {
        uart_conf.data_bits = DATA_BITS_9;
        uart_conf.parity = PARITY_EVEN;
        break;
    }
    }

    /* open serial device */
    if (RT_EOK == rt_device_open(serial_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX)) {
        /* set serial configure */
        if(rt_device_control(serial_dev, RT_DEVICE_CTRL_CONFIG, &uart_conf) == RT_EOK)
        {
            serial = (struct rt_serial_device*)serial_dev;
            rt_kprintf("ulBaudRate:%u\n", serial->config.baud_rate);
            rt_kprintf("data_bits:%u\n", serial->config.data_bits);
            rt_kprintf("stop_bits:%u\n", serial->config.stop_bits);
            rt_kprintf("parity:%u\n\n", serial->config.parity);

        }else{
            rt_kprintf("rt_device_control failed\n");
        } 
        rt_device_set_rx_indicate(serial_dev, serial_rx_ind);
    } else {
        rt_kprintf("%s open failed\n", uart_name);
        return FALSE;
    }

    /* software initialize */
    rt_event_init(&event_serial, "master event", RT_IPC_FLAG_PRIO);
    rt_thread_init(&thread_serial_soft_trans_irq,
                   "master trans",
                   serial_soft_trans_irq,
                   RT_NULL,
                   serial_soft_trans_irq_stack,
                   sizeof(serial_soft_trans_irq_stack),
                   10, 5);
    rt_thread_startup(&thread_serial_soft_trans_irq);

    return TRUE;
}

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    rt_uint32_t recved_event;
    if (xRxEnable)
    {
        /* enable RX interrupt */
        serial_dev->ops->control(serial_dev, RT_DEVICE_CTRL_SET_INT, (void *)RT_DEVICE_FLAG_INT_RX);
        /* switch 485 to receive mode */
#if defined(RT_MODBUS_MASTER_USE_CONTROL_PIN)
        rt_pin_write(MODBUS_MASTER_RT_CONTROL_PIN_INDEX, PIN_LOW);
#endif
    }
    else
    {
        /* switch 485 to transmit mode */
#if defined(RT_MODBUS_MASTER_USE_CONTROL_PIN)
        rt_pin_write(MODBUS_MASTER_RT_CONTROL_PIN_INDEX, PIN_HIGH);
#endif
        /* disable RX interrupt */
        serial_dev->ops->control(serial_dev, RT_DEVICE_CTRL_CLR_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    }
    if (xTxEnable)
    {
        /* start serial transmit */
        rt_event_send(&event_serial, EVENT_SERIAL_TRANS_START);
    }
    else
    {
        /* stop serial transmit */
        rt_event_recv(&event_serial, EVENT_SERIAL_TRANS_START,
                RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, 0,
                &recved_event);
    }
}

void vMBMasterPortClose(void)
{
    // serial->parent.close(&(serial->parent));
    serial_dev->ops->close(serial_dev);
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
    // serial->parent.write(&(serial->parent), 0, &ucByte, 1);
    serial_dev->ops->write(serial_dev, 0, &ucByte, 1);

    return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
    // serial->parent.read(&(serial->parent), 0, pucByte, 1);
    serial_dev->ops->read(serial_dev, 0, pucByte, 1);
    return TRUE;
}

/*
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBMasterFrameCBTransmitterEmpty();
}

/*
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBMasterFrameCBByteReceived();
}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
static void serial_soft_trans_irq(void* parameter) {
    rt_uint32_t recved_event;
    while (1)
    {
        /* waiting for serial transmit start */
        rt_event_recv(&event_serial, EVENT_SERIAL_TRANS_START, RT_EVENT_FLAG_OR,
                RT_WAITING_FOREVER, &recved_event);
        /* execute modbus callback */
        prvvUARTTxReadyISR();
    }
}

/**
 * This function is serial receive callback function
 *
 * @param dev the device of serial
 * @param size the data size that receive
 *
 * @return return RT_EOK
 */
static rt_err_t serial_rx_ind(rt_device_t dev, rt_size_t size) {
    prvvUARTRxISR();
    return RT_EOK;
}

#endif
