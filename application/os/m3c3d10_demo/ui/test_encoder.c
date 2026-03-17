#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "aic_hal_gpio.h"

#define GET_PIN(PORT, PIN) (GPIO_GROUP_SIZE * (P##PORT##_GROUP)  + PIN)

// 定义编码器的 GPIO 引脚
#define ENCODER_A_PIN    GET_PIN(D, 13)
#define ENCODER_B_PIN    GET_PIN(D, 12)

// 定义一个变量来存储编码器的计数值
volatile int32_t encoder_count = 0;

// 用于记录上一次的编码器状态
static uint8_t last_state = 0;

// 状态表定义
const int8_t state_table[16] = {
    0,  // 0000
    -1, // 0001
    1,  // 0010
    0,  // 0011
    1,  // 0100
    0,  // 0101
    0,  // 0110
    -1, // 0111
    -1, // 1000
    0,  // 1001
    0,  // 1010
    1,  // 1011
    0,  // 1100
    1,  // 1101
    -1, // 1110
    0   // 1111
};

// 中断服务函数
void encoder_isr(void *args)
{
    // 读取当前 A 和 B 通道的状态
    rt_bool_t a = rt_pin_read(ENCODER_A_PIN);
    rt_bool_t b = rt_pin_read(ENCODER_B_PIN);

    // 当前状态组合
    uint8_t current_state = (a << 1) | b;

    // 计算状态变化
    uint8_t state_change = ((last_state << 2) | current_state) & 0x0F;

    // 更新计数值
    encoder_count += state_table[state_change];

    // 更新上一次的状态
    last_state = current_state;
}

int test_encoder(int argc, char *argv[])
{
    // 配置 A 和 B 引脚为输入模式

    rt_pin_mode(ENCODER_A_PIN, PIN_MODE_INPUT);
    rt_pin_mode(ENCODER_B_PIN, PIN_MODE_INPUT);

    // 初始化上一次的状态
    last_state = (rt_pin_read(ENCODER_A_PIN) << 1) | rt_pin_read(ENCODER_B_PIN);

    // 使能 A 引脚的中断，并设置中断服务函数
    rt_pin_attach_irq(ENCODER_A_PIN, PIN_IRQ_MODE_RISING_FALLING, encoder_isr, RT_NULL);
    rt_pin_irq_enable(ENCODER_A_PIN, PIN_IRQ_ENABLE);

    // 使能 B 引脚的中断，并设置中断服务函数
    rt_pin_attach_irq(ENCODER_B_PIN, PIN_IRQ_MODE_RISING_FALLING, encoder_isr, RT_NULL);
    rt_pin_irq_enable(ENCODER_B_PIN, PIN_IRQ_ENABLE);

#if 0
    int tranfer_data = 0;
    int _encode_count = 0;
    while (1)
    {
        // 打印编码器的计数值
        tranfer_data = encoder_count / 4;
        if(tranfer_data != _encode_count)
        {
            rt_kprintf("Encoder count: %d\n", tranfer_data);
            _encode_count = tranfer_data;
        }
        rt_thread_mdelay(10);
    }
#endif
}


MSH_CMD_EXPORT(test_encoder, test_encoder);
