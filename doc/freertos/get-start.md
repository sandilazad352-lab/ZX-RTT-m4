# SDK + FreeRTOS Development Guide

## Introduction to FreeRTOS

[FreeRTOS](https://www.freertos.org/) is a leading real-time operating system (RTOS) offering small, low-power solutions for microcontrollers. It's ideal for low-cost, low-power devices, such as IoT devices, wearable technology, and embedded systems.

### Default Configuration

- **Version**: V10.4.3 LTS Patch 3
- **Heap Management**: Heap_5
- **System Clock Frequency**: 500Hz
- **Scheduling Policy**: Preemptive scheduling

### Memory Allocation

- Use `pvPortMalloc` for dynamic memory allocation, with memory located in the SRAM area.

## SDK Driver Support Based on FreeRTOS

### HAL Layer Drivers

| Driver Type | Description              |
|-------------|--------------------------|
| GPIO        | General Purpose Input/Output Ports |
| CAN         | Controller Area Network  |
| GPAI        | General Parallel AI Interface |
| PWM         | Pulse Width Modulation   |
| I2C         | Thread-safe Serial Bus   |
| UART        | Thread-safe Serial Communication |

### Bare Layer Drivers

- **Audio**: Audio playback and recording
- **EFuse**: Electronic fuse for storing device configuration
- **HRTimer**: High Resolution Timer
- More drivers available in the `drv_bare` folder.

### Additional Support

- **Button**: Supports ADC Button and GPIO Button.
- **Touch**: Supports various touch screen controllers, such as GT911 and FT5X06, etc.

## Getting Started

1. **Review Configuration Files**

   Run `Scons --list-def` to see all available configuration files.

2. **Select the Corresponding FreeRTOS Configuration**

   Use `Scons --apply-def=ZXM37D0_freertos_defconfig` to select a suitable FreeRTOS configuration.

3. **Compile**

   Run `Scons` to start compiling the project.
