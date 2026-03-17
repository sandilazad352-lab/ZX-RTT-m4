
# CAN驱动使用文档

## 简介

CAN（Controller Area Network）驱动允许您配置和使用CAN通信接口，支持消息发送、接收、过滤和状态获取等功能。此驱动适用于需要通过CAN协议进行设备间通信的应用。

## 配置和API说明

### CAN设备初始化

```c
can_handle_t* can_init(can_index_t index, can_config_t* config);
```

### 发送CAN消息

```c
int can_write_frame(can_handle_t* handle, can_msg_t* msg, can_op_req_t req);
```

### 设置CAN过滤器

```c
int can_set_filter(can_handle_t* handle, can_filter_config_t* filter);
```

### 获取CAN状态

```c
int can_get_status(can_handle_t* handle, can_status_t* status);
```

### 接收CAN消息

```c
int can_recv_msg(can_handle_t* handle, can_msg_t* msg, uint32_t timeout_ms);
```

## 示例用法

### 循环接收CAN消息

以下示例展示了如何初始化CAN设备，配置接收队列，并循环接收CAN消息。

```c
#include "can.h"

int main() {
    // 初始化CAN设备
    can_config_t config = {
        .baudrate = 500000, // 设置波特率为500Kbps
        .mode = CAN_MODE_NORMAL, // 正常模式
        .rx_queue_len = 10 // 接收队列长度为10
    };
    can_handle_t* handle = can_init(CAN_0, &config);

    if (handle == NULL) {
        printf("CAN初始化失败。\n");
        return -1;
    }

    // 循环接收消息
    can_msg_t msg;
    while (1) {
        if (can_recv_msg(handle, &msg, 5000) == 0) { // 5秒超时
            printf("接收到CAN消息：ID = %x, 数据 = ", msg.id);
            for (int i = 0; i < 8; ++i) {
                printf("%02x ", msg.data[i]);
            }
            printf("\n");
        } else {
            printf("接收CAN消息超时。\n");
        }
    }
}
```

> **_NOTE:_** 使用前请确保CAN引脚已经配置好复用
