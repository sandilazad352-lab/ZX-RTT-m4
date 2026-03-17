# CAN Driver Documentation

## Introduction

The CAN (Controller Area Network) driver enables you to configure and use the CAN communication interface, supporting message sending, receiving, filtering, and status retrieval. This driver is suitable for applications that require inter-device communication via the CAN protocol.

## Configuration and API Description

### CAN Device Initialization

```c
can_handle_t* can_init(can_index_t index, can_config_t* config);
```

### Send CAN Message

```c
int can_write_frame(can_handle_t* handle, can_msg_t* msg, can_op_req_t req);
```

### Set CAN Filter

```c
int can_set_filter(can_handle_t* handle, can_filter_config_t* filter);
```

### Get CAN Status

```c
int can_get_status(can_handle_t* handle, can_status_t* status);
```

### Receive CAN Message

```c
int can_recv_msg(can_handle_t* handle, can_msg_t* msg, uint32_t timeout_ms);
```

## Example Usage

### Loop to Receive CAN Messages

The following example shows how to initialize a CAN device, configure a receive queue, and loop to receive CAN messages.

```c
#include "can.h"

int main() {
    // Initialize CAN device
    can_config_t config = {
        .baudrate = 500000, // Set baud rate to 500Kbps
        .mode = CAN_MODE_NORMAL, // Normal mode
        .rx_queue_len = 10 // Receive queue length of 10
    };
    can_handle_t* handle = can_init(CAN_0, &config);

    if (handle == NULL) {
        printf("CAN initialization failed.\n");
        return -1;
    }

    // Loop to receive messages
    can_msg_t msg;
    while (1) {
        if (can_recv_msg(handle, &msg, 5000) == 0) { // 5-second timeout
            printf("Received CAN message: ID = %x, Data = ", msg.id);
            for (int i = 0; i < 8; ++i) {
                printf("%02x ", msg.data[i]);
            }
            printf("\n");
        } else {
            printf("Receiving CAN message timed out.\n");
        }
    }
}
```

> **_NOTE:_** Ensure the CAN pins are properly configured for multiplexing before use.