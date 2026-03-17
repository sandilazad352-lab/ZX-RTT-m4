# General Purpose Analog Input (GPAI) Driver Documentation

## Introduction

The GPAI (General Purpose Analog Input) driver provides a set of APIs that allow users to configure and read the values of analog input channels. This driver supports multi-channel input, suitable for applications requiring analog signal acquisition.

## Configuration and API Description

### Initialize the GPAI System

Before using any other GPAI functions, you must first call the following function to prepare the GPAI hardware:

```c
int gpai_init(void);
```

### Configure GPAI Channels

You can enable or disable a specified GPAI channel and configure its sampling mode and period with the following function:

```c
int gpai_enabled(uint32_t ch, uint8_t enabled, enum aic_gpai_mode mode, uint32_t period_ms);
```

Parameter description:

- `ch`: The channel number to configure.
- `enabled`: Set to 1 to enable the channel, or 0 to disable it.
- `mode`: The sampling mode.
- `period_ms`: The sampling period, in milliseconds.

### Read Analog Input Value

You can read the current analog value from a specified GPAI channel and convert it to millivolts with the following function:

```c
int gpai_read_mv(uint32_t ch);
```

Parameter description:

- `ch`: The channel number to read.

### Get GPAI System Resolution

Get the resolution of the GPAI system with the following function:

```c
int gpai_get_resolution();
```

## Example Usage

### Initialize GPAI

At the beginning of the program, first initialize the GPAI system:

```c
#include "gpai.h"

int main() {
    gpai_init();
    // You can then configure GPAI channels and read values
}
```

### Configure and Read a GPAI Channel

Enable a GPAI channel and read its analog value:

```c
#include "gpai.h"

int main() {
    // Initialize the GPAI system
    gpai_init();
    
    // Enable channel 0, set to single sampling mode, sampling period of 100ms
    gpai_enabled(0, 1, AIC_GPAI_MODE_SINGLE, 100);
    
    // Read and print the analog value of channel 0 (in millivolts)
    int mv = gpai_read_mv(0);
    printf("Channel 0 reads %d mV\n", mv);
}
```

### Continuously Read a GPAI Channel

After enabling a GPAI channel, you can continuously read its analog value in a loop:

```c
#include "gpai.h"
#include <unistd.h> // For using the sleep function

int main() {
    // Initialize the GPAI system
    gpai_init();
    
    // Enable channel 0, set to continuous sampling mode, sampling period of 100ms
    gpai_enabled(0, 1, AIC_GPAI_MODE_CONTINUOUS, 100);
    
    // Continuously read and print the analog value of channel 0 (in millivolts)
    while(1) {
        int mv = gpai_read_mv(0);
        printf("Channel 0 reads %d mV\n", mv);
        vTaskDelay(pdMS_TO_TICKS(100)); // Read every 100ms
    }
}
```

> **_NOTE:_** Ensure the GPAI pins are properly configured for multiplexing before use.