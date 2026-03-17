#ifndef __BSP_UART_H_
#define __BSP_UART_H_
#include <rtthread.h>
typedef struct
{
    void (*f_init)(void (*callback)(unsigned char *buff));
    void (*f_callback_analysis)(uint8_t *Rx_Data_Buff);
} S_BSP_UART;
extern S_BSP_UART bsp_uart;

int wft_prl_serial_init(void (*callback)(unsigned char *buff));

#endif