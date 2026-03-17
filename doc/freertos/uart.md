# ZX UART Driver Documentation

## 1. Introduction

The ZX UART driver provides an interface for initializing and managing UART (Universal Asynchronous Receiver/Transmitter) communication on supported hardware. It is designed to offer developers a flexible and efficient way to implement serial data transmission.

## 2. Configuration and API Description

### Configuration Structure

- `uart_config_t`: This structure defines the configuration parameters for a UART device, including baud rate, data bits, stop bits, parity mode, UART mode, receive FIFO length, receive notification length, and receive interrupt type.

### Initialization

- `zx_uart_init(uart_num_t uart_num, uart_config_t* config)`: Initializes the specified UART module with the given configuration.

### Data Writing

- `zx_uart_write(uart_num_t num, uint8_t* data, uint32_t len)`: Writes data to UART.
- `zx_uart_write_wait_done(uart_num_t num)`: Waits for all queued UART write operations to complete.
- `zx_uart_write_all(uart_num_t num, uint8_t* data, uint32_t len)`: Writes the specified data to UART and blocks until completion.
- `zx_uart_write_byte(uart_num_t num, uint8_t byte)`: Writes a single byte to UART.

### Data Reading

- `zx_uart_rx_available(uart_num_t num)`: Returns the number of bytes available to read from UART.
- `zx_uart_read(uart_num_t num, uint8_t* data, uint32_t len)`: Reads data from UART.
- `zx_uart_wait_rx_data(uart_num_t num, uint32_t timeout_ms)`: Waits for a specified amount of data to become available on UART.

### FIFO Management

- `zx_uart_clean_rx_fifo_overflow(uart_num_t num)`: Clears the UART receive FIFO overflow counter.
- `zx_uart_clean_rx_fifo(uart_num_t num)`: Clears the UART receive FIFO buffer.

## 3. Example

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

> **_NOTE:_** Ensure the serial port pins are properly configured for multiplexing before use.