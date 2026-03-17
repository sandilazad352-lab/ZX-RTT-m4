/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2018-08-15   misonyo         first implementation.
 * 2023-05-25   geo.dong        ZX
 */

#include <rtthread.h>
#include <aic_core.h>

typedef struct
{
    int status; // 1 - 收发通过
    struct rt_semaphore rx_sem;
    rt_device_t dev;
} BUSx_ATTR;

static const char m_send[] = "1234567890MASTER1234567890\n";
static const char s_send[] = "1234567890SLAVE1234567890\n";

BUSx_ATTR *uart1_attr;
BUSx_ATTR *uart3_attr;

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    struct rt_semaphore *rx_sem;
    if(dev == uart1_attr->dev)
        rx_sem = &uart1_attr->rx_sem;
    else if(dev == uart3_attr->dev)
        rx_sem = &uart3_attr->rx_sem;
    else
        return RT_ERROR;
    if (size > 0)
        rt_sem_release(rx_sem);

    return RT_EOK;
}
static void serial_rx_thread(void *parameter)
{
    BUSx_ATTR *attr = (BUSx_ATTR *)parameter;
    char ch;
    int ret = 0;
    char str_receive[128] = {0};
    int index = 0;
    while (1)
    {
        ret = rt_device_read(attr->dev, -1, &ch, 1);
        if (ret > 0) {
            str_receive[index] = ch;
            index ++;
            if((index >= 1) && (str_receive[0] != '1'))
            {
                ch = '\0';
                index = 0;
            }
            if(index == 128 || ch == '\n')
            {
                printf("%s receive: %s \n", attr == uart1_attr ? "uart1" : "uart3", str_receive);
                rt_memset(str_receive,  0, sizeof(str_receive));
                index = 0;
            }
        } else {
            rt_sem_take(&attr->rx_sem, RT_WAITING_FOREVER);
            rt_thread_delay(20);
        }
    }

}

static void serial_tx_thread(void *parameter)
{
    static int sel = 0;
    while (1)
    {
        if(sel)
            rt_device_write(uart1_attr->dev, 0, s_send, (sizeof(s_send) - 1));
        else
            rt_device_write(uart3_attr->dev, 0, s_send, (sizeof(s_send) - 1));
        rt_thread_delay(1500);
        sel = !sel;
    }

}

BUSx_ATTR *uart_init(char *uart_name)
{
    rt_err_t ret = RT_EOK;

    BUSx_ATTR *attr = (BUSx_ATTR *)rt_malloc(sizeof(BUSx_ATTR));
    rt_memset(attr, 0, sizeof(BUSx_ATTR));

    if(attr == NULL)
        goto err;

    attr->dev = rt_device_find(uart_name);
    if (!attr->dev)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        goto err;
    }

    rt_sem_init(&attr->rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    ret = rt_device_open(attr->dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    if (ret != RT_EOK)
    {
        rt_kprintf("open %s failed : %d !\n", uart_name, ret);
        goto err;
    }

    rt_device_set_rx_indicate(attr->dev, uart_input);

    char thread_name[128];

    snprintf(thread_name, sizeof(thread_name), "%s_rx", uart_name);
    
    // NOTE: thread stack-size at least for 1024*2 Bytes !!!
    rt_thread_t rx_thread;

    rx_thread = rt_thread_create(thread_name, serial_rx_thread, attr, 1024*2, 25, 10);
    
    if (rx_thread != RT_NULL)
        rt_thread_startup(rx_thread);
    else
        goto err;

    snprintf(thread_name, sizeof(thread_name), "%s_tx", uart_name);

    return attr;


err:
    if(attr)
    {
        rt_free(attr);
        attr = RT_NULL;
    }
    return attr;
}

int test_rs485(int argc, char *argv[])
{
    uart1_attr = uart_init("uart1");
    uart3_attr = uart_init("uart3");


    rt_thread_t tx_thread;

    tx_thread = rt_thread_create("uart_send", serial_tx_thread, NULL, 1024*2, 25, 10);
    if (tx_thread != RT_NULL)
        rt_thread_startup(tx_thread);
}


MSH_CMD_EXPORT(test_rs485, ZX UART Test);