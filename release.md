# V1.3.0 #
- 优化DMA
- 支持avi封装格式
- PSRAM：优化初始化参数，提升稳定性
- ADC：优化校准参数的算法
- SPI NAND：优化内存拷贝流程；减少数据传输中的delay；
- SPI：优化DMA传输
- UART：修正485模式的数据传输错误
- M3C/M3A: 支持HRTIMER

# V1.2.1 #
- 增加base_demo，播放视频
- 增加freertos文档
- 增加ZXM3A7D0

# V1.2.0 #
- 支持串口DMA
- 修复挂载多个FATFS失败的问题
- freertos支持I2C、PWM、GPAI和GPIO驱动和examples

# V1.1.0 #
## 新增 ##
- 支持4.45 MIPI GV503 LCD
- 支持freertos V10.4.3
- 支持仪表meter demo
- 支持OTA升级方案(ymodem和http)
- 增加一些AVL设备支持
## 优化 ##
- UART烧写：优化稳定性
- 优化内置的PSRAM的稳定性

# V1.0.2 #
## 新增 ##
- 支持ZX3D00
- 新增widgets app

# V1.0.1 #
## 新增 ##
- 支持Model 3C,Model 3A
- 新增CAP和EPWM支持

# V1.0.0 #
## 新增 ##
- 初始稳定版
- 支持Model 4,Model 3
- lvgl 8.3.10

