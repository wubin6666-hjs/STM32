# STM32F103C8T6 练习项目集合

基于 STM32F103C8T6 + 标准外设库（StdPeriph Library）+ Keil MDK5 的裸机与 FreeRTOS 学习项目集合。

## 开发环境

- MCU：STM32F103C8T6（Cortex-M3，72 MHz，64 KB Flash / 20 KB RAM）
- 库：STM32F10x Standard Peripheral Library
- IDE：Keil MDK5 + ST-Link / J-Link（SWD）
- 调试：示波器、逻辑分析仪

## 项目列表

| 目录 | 说明 |
| --- | --- |
| `freertos_ADC/` | **FreeRTOS 多任务 ADC 采集系统**：4 个任务（启动/命令/采集显示/心跳），消息队列投递串口命令，互斥量保护串口打印，临界区保护 DMA 数据；实现栈溢出与堆失败钩子。 |
| `AD-DMA/`、`ADC多/`、`ADC/`、`DMA/` | **裸机 ADC 采集**：TIM3 TRGO 触发 ADC，DMA 循环搬运 12 位转换结果，滑动平均滤波，阈值告警。 |
| `软件I2C/`、`硬件I2C/` | **MPU6050 六轴姿态传感器**：分别用 GPIO 模拟时序与硬件 I2C 外设实现两套主机驱动，读取三轴加速度、三轴陀螺仪与温度，OLED 实时显示。 |
| `PWM舵机/` | TIM2 PWM 输出（50 Hz）驱动舵机，按键切换占空比实现角度调节。 |
| `定时器 -编码器/`、`定时器输入/` | EXTI 外部中断（PB14 下降沿）脉冲计数与 NVIC 分组配置。 |
| `串口/`、`串口发送/`、`9-2 串口发送+接收/` | USART 中断收发、printf 重定向、帧状态机解析。 |
| `按键控制LED/`、`LED闪烁/`、`work1/` | GPIO / EXTI 入门练习。 |

## 简历对应关系

| 简历项目 | 对应目录 |
| --- | --- |
| 多通道 ADC 采集与串口交互系统（裸机版） | `ADC多/`、`AD-DMA/`、`串口/` |
| FreeRTOS ADC 采集多任务系统 | `freertos_ADC/` |
| MPU6050 六轴传感器采集 | `软件I2C/`、`硬件I2C/` |
| 按键控制舵机与外部中断计数 | `PWM舵机/`、`定时器 -编码器/` |

## 备注

- 每个工程都自带 `library/`（标准外设库源码）与启动文件，下载后可直接用 Keil 打开编译。
- 编译产物（`Objects/`、`Listings/`、`DebugConfig/`）已在 `.gitignore` 中排除。
