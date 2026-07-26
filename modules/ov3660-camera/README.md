---
type: module
name: OV3660 Camera
slug: ov3660-camera
vendor: generic
aliases: [OV3660, OmniVision OV3660]
device: OV3660
interface: [DVP-parallel, SCCB]
date_added: 2025-07-21
used_by:
  - m5stack-timer-cameraf
sources:
  - raw/datasheets/m5stack-timercam-f_pinmap.txt
tags: [module, camera, ov3660, dvp]
---

# OV3660 Camera

> OmniVision **OV3660**：1/5" CMOS，约 314 万像素（2048×1536），支持 RAW/RGB/YUV 输出，DVP 并行 + SCCB 控制。是 ESP32 类 DVP 摄像头板的常用传感器（如 M5 Timer Camera F）。

> 本页是 **OV3660 器件信息的事实源**（器件特性、信号定义、接口）。具体某块板把这些信号接到哪些 GPIO，见各板页面（例如 [Timer Camera F](../../boards/m5stack-timer-cameraf/README.md) 的「摄像头 DVP 接线」表）。

## 概览

- 器件：OmniVision OV3660
- 有效像素：2048 × 1536（≈3MP）
- 光学尺寸：1/5"
- 输出格式：RAW / RGB(565/888) / YUV(422)
- 帧率：传感器规格最高 2048×1536 @ **15fps**（1080p @ 20fps、720p @ 45fps；实际还受 ESP32 DVP 吞吐限制）
- 镜头：模组封装时搭配镜头（如 TimerCam F 为 120° 鱼眼）

## 核心器件

- 像素阵列：2048×1536
- 供电：核心 1.5V / 模拟 1.8V 或 2.8V / I/O(DOVDD) 1.8V 或 3.3V。**模组通常板载 LDO，对外只给 3.3V 即可**。
- 功耗：低功耗模式支持（PWDN / 待机）

## 引脚定义（传感器信号）

> 这些是 OV3660 的**信号定义**；板子把它们接到具体 GPIO 的对应关系在各板页面（DVP 接线表）。

| 信号 | 方向 | 说明 |
|---|---|---|
| D0–D9 | I/O | 像素数据；ESP32 DVP 通常用 8 位 **D0–D7** |
| VSYNC | O | 帧同步 |
| HREF | O | 行有效 |
| PCLK | O | 像素时钟 |
| XCLK | I | **外部主时钟输入**（ESP32 侧给，典型 16–24MHz） |
| SIOC | I/O | SCCB 时钟（I2C-like 的 SCL） |
| SIOD | I/O | SCCB 数据（I2C-like 的 SDA） |
| RESET# | I | 复位（低有效） |
| PWDN | I | 掉电（高有效） |
| DOVDD | P | I/O 电源 1.8V/3.3V |
| DVDD | P | 核心电源 1.5V（板载 LDO） |
| AVDD | P | 模拟电源（板载 LDO） |
| GND | P | 地 |

## 接口

- **DVP 并行**：8 位数据 + VSYNC/HREF/PCLK 帧时序，ESP32 用 esp32-camera 驱动读取。
- **XCLK**：需由 MCU 提供主时钟（LEDC 生成 16–24MHz 方波）。
- **SCCB**：I2C-like，用于配置传感器寄存器（分辨率、格式、增益、白平衡…）。默认 7 位地址 `0x3C`（待核实，不同批次/模组可能不同，esp32-camera 驱动会探测 ID）。
- OV3660 亦支持 2-lane MIPI CSI，但 ESP32（非 S3/带 CSI 的）DVP 板只用并行口。

## 引脚物理排布

- 裸片为 CSP 封装，模组形态各异（FPC / 焊接在底板上）。物理排布以**具体载板**为准，见使用本器件的板子页面。

## 示例代码

- 驱动：Espressif [esp32-camera](https://github.com/espressif/esp32-camera) 库，`sensor_t` 设置为 `OV3660_PID`。
- 点亮示例：见使用本模组的板子页面（如 [Timer Camera F](../../boards/m5stack-timer-cameraf/README.md) 的 `examples/`，含按板级 DVP 接线配置好的 `camera_config`）。

## 被哪些开发板使用

| 板子 | 角色 | 链接 |
|---|---|---|
| M5Stack Timer Camera F | 板载摄像头（120° 鱼眼） | [板子页](../../boards/m5stack-timer-cameraf/README.md) |

## 相关页面 / 参考来源

- OmniVision OV3660 preliminary specification v1.3（本次已复核满分辨率为 15fps；TODO：存入 `raw/datasheets/`）
- esp32-camera 驱动：<https://github.com/espressif/esp32-camera>
