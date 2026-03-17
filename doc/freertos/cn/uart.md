
# ZX UART驱动程序文档

## 1. 简介

ZX UART驱动程序提供了一个接口，用于在支持的硬件上初始化和管理UART（通用异步收发传输器）通信。它旨在为开发人员提供灵活、高效的方式来实现串行数据传输。

## 2. 配置和API说明

### 配置结构体

- `uart_config_t`：此结构体定义了UART设备的配置参数，包括波特率、数据位、停止位、奇偶校验模式、UART模式、接收FIFO长度、接收通知长度和接收中断类型。

### 初始化

- `zx_uart_init(uart_num_t uart_num, uart_config_t* config)`：使用给定的配置初始化指定的UART模块。

### 数据写入

- `zx_uart_write(uart_num_t num, uint8_t* data, uint32_t len)`：将数据写入UART。
- `zx_uart_write_wait_done(uart_num_t num)`：等待所有排队的UART写操作完成。
- `zx_uart_write_all(uart_num_t num, uint8_t* data, uint32_t len)`：写入指定的数据到UART，并阻塞直到完成。
- `zx_uart_write_byte(uart_num_t num, uint8_t byte)`：写入单个字节到UART。

### 数据读取

- `zx_uart_rx_available(uart_num_t num)`：返回可从UART读取的字节数。
- `zx_uart_read(uart_num_t num, uint8_t* data, uint32_t len)`：从UART读取数据。
- `zx_uart_wait_rx_data(uart_num_t num, uint32_t timeout_ms)`：等待指定数量的数据在UART上可用。

### FIFO管理

- `zx_uart_clean_rx_fifo_overflow(uart_num_t num)`：清除UART接收FIFO溢出计数器。
- `zx_uart_clean_rx_fifo(uart_num_t num)`：清除UART接收FIFO缓冲区。

## 3. 示例

```c
int main(void)
{

    uart_config_t uart_config = UART_DEFAULT_CONIFG;
    uart_config.baud_rate = 115200;
    int ret = zx_uart_init(UART_NUM_3, &uart_config);
    if (ret != 0) {
        printf("uart_protocol_init failed: %d\r\n", ret);
    }
    printf("Init success \r\n");
    uint8_t test[512] = {0x0};
    for (;;) {
        int len = zx_uart_wait_rx_data(UART_NUM_3, 100000);
        zx_uart_read(UART_NUM_3, test, len);
        
        zx_uart_write(UART_NUM_3, test, len);

        printf("recv [%d]: ", len);
        for (uint16_t i = 0; i < len; i++) {
            printf("%02x ", test[i]);
        }
        printf("\r\n");
    }
    return 0;
}

```

> **_NOTE:_** 使用前请确保串口引脚已经配置好复用
