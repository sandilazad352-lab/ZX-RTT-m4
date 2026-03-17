/*
 * 程序清单：这是一个 串口 设备使用例程
 * 例程导出了 uart_sample 命令到控制终端
 * 命令调用格式：uart_sample uart2
 * 命令解释：命令第二个参数是要使用的串口设备名称，为空则使用默认的串口设备
 * 程序功能：通过串口输出字符串"hello RT-Thread!"，然后错位输出输入的字符
 */

#include <aic_core.h>
#include <rtthread.h>

#include "bsp_uart.h"
S_BSP_UART bsp_uart = {
    .f_init = wft_prl_serial_init
};


#define SAMPLE_UART_NAME "uart1"

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
rt_device_t wft_prl_serial;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    int data_len;
    unsigned char data_buff[300];
    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        // while (rt_device_read(serial, -1, &ch, 1) != 1)
        // {
        //     /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
        //     rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        //     aicos_msleep(100);
        // }
        /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        aicos_msleep(10);
        data_len = rt_device_read(wft_prl_serial, -1, data_buff, 300);

        if (data_len > 0)
        {
            if (bsp_uart.f_callback_analysis != NULL)
            {
                bsp_uart.f_callback_analysis(data_buff);
            }
            /* 读取到的数据通过串口错位输出 */
            // ch = ch + 1;
            // rt_device_write(wft_prl_serial, 0, data_buff, data_len);
        }
    }
}

#define WFT_SERIAL_THREAD_PRIORITY 20
#define WFT_SERIAL_THREAD_STACK_SIZE 2048
#define WFT_SERIAL_THREAD_TIMESLICE 5

ALIGN(RT_ALIGN_SIZE)
static char wft_serial_hread_stack[WFT_SERIAL_THREAD_STACK_SIZE];
static struct rt_thread wft_prl_serial_thread;


int wft_prl_serial_init(void (*callback)(unsigned char *buff))
{
    rt_err_t ret = RT_EOK;
    // char uart_name[RT_NAME_MAX];
    // char str[] = "hello RT-Thread!\r\n";

    // if (argc == 2)
    // {
    //     rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    // }
    // else
    // {
    //     rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    // }
    bsp_uart.f_callback_analysis = callback;
    /* 查找系统中的串口设备 */
    wft_prl_serial = rt_device_find(SAMPLE_UART_NAME);
    if (!wft_prl_serial)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(wft_prl_serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(wft_prl_serial, uart_input);
    /* 发送字符串 */
    // rt_device_write(wft_prl_serial, 0, str, (sizeof(str) - 1));

    /* 创建 serial 线程 */
    // rt_thread_t thread = rt_thread_create("wft_serial", serial_thread_entry, RT_NULL, 512, 20, 5);

    rt_thread_init(&wft_prl_serial_thread,
                   "wft_serial",
                   serial_thread_entry,
                   RT_NULL,
                   &wft_serial_hread_stack[0],
                   WFT_SERIAL_THREAD_STACK_SIZE,
                   WFT_SERIAL_THREAD_PRIORITY,
                   WFT_SERIAL_THREAD_TIMESLICE);
        rt_thread_startup(&wft_prl_serial_thread);
    rt_kprintf("wft_prl_serial_thread init ok ! \r\n");



    return ret;
}
