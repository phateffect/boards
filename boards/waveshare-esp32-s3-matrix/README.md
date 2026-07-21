---
type: board
name: Waveshare ESP32-S3-Matrix
slug: waveshare-esp32-s3-matrix
vendor: Waveshare
aliases: [ESP32-S3-Matrix]
chip: ESP32-S3
references_modules: []
onboard_modules:
  - 8×8 WS2812 RGB LED matrix (64 LEDs)
date_added: 2025-07-21
sources:
  - raw/datasheets/waveshare-esp32-s3-matrix.txt
tags: [board, esp32-s3, waveshare, led-matrix]
---

# Waveshare ESP32-S3-Matrix

> Waveshare 出的紧凑型 ESP32-S3 开发板，板载 **8×8 = 64 颗 WS2812 全彩 LED 矩阵**，20 个 GPIO 排针引出，USB-C 供电/烧录。适合做像素动画、小游戏、状态显示。

> ⚠️ 规格与矩阵引脚已据官方 wiki 核实；**20 个引出 GPIO 的逐脚定义在官方 Pinout 图中（图片），尚未录入**，下方"引出引脚"表多为 TODO，待把引脚图存入 `raw/photos/` 后补全。

## 概览

- 厂商 / 型号：Waveshare / ESP32-S3-Matrix
- 资料：<https://www.waveshare.com/wiki/ESP32-S3-Matrix>

## 主控芯片

- **ESP32-S3**（Xtensa 32-bit LX7 双核，最高 240MHz）
- SRAM 512KB · ROM 384KB · RTC SRAM 16KB
- Flash **4MB**（板载，无 PSRAM）
- Wi-Fi 2.4GHz + BLE

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 8×8 RGB LED 矩阵 (WS2812) | **G14** | 单线 RGB 数据（级联 64 颗） | Arduino core 中即 `PIN_NEOPIXEL=14`，用 `neopixelWrite()` 或 Adafruit NeoPixel/NeoMatrix 驱动 |
| BOOT 按键 | (strapping GPIO0) | BOOT | 进入下载模式：按住 BOOT 再按 RESET |
| RESET 按键 | EN/CHIP_PU | 复位 | — |
| USB-UART / USB | USB（D+/D-） | USB-CDC | USB Type-C；部分系统需手装驱动；烧录选 UART 或 USB |

> 矩阵亮度不宜过高：官方提示高亮度会快速升温、可能损坏板子。

## 引出引脚

> 20 个 GPIO 通过排针引出。**逐脚定义待官方 Pinout 图录入后补全**（TODO）。下表为占位骨架。

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 3V3 | 3V3 | 电源 | — |
| GND | GND | 地 | — |
| ? | GPIO? | — | TODO：按官方 Pinout 图补 |
| _…（共 20 个 GPIO 排针）_ | | | TODO |

## 引脚物理排布

> TODO：按官方 Pinout 图补 ASCII 排布（板子两侧排针，USB-C 一侧朝上，标 Pin1）。

## 原理图

- 官方 wiki 提供原理图与 Pinout 图：<https://www.waveshare.com/wiki/ESP32-S3-Matrix>
- TODO：下载到 `raw/schematics/waveshare-esp32-s3-matrix.pdf`、`raw/photos/...pinout.png`

## 示例代码

- [`examples/matrix_smile/`](examples/matrix_smile/) — 用 GPIO14 驱动 8×8 矩阵，画一个笑脸/红点

## 特殊说明

- 烧录/下载失败常见解法：按住 RESET >1s 后重试；或进下载模式（按住 BOOT + 按 RESET，先松 RESET）。
- 串口/USB 反复重启：见官方 FAQ（多为供电或驱动问题）。
- 矩阵驱动建议用 Adafruit **NeoPixel** / **NeoMatrix** 库；Arduino-ESP32 亦内置 `neopixelWrite(PIN_NEOPIXEL, r, g, b)`。

## 相关页面

- 同芯片(ESP32-S3) 的板子：_(暂无)_
- 暂无引用模组

## 参考来源

- `raw/datasheets/waveshare-esp32-s3-matrix.txt`（官方 wiki 页面文本快照，2025-07-21 抓取）
- 官方 wiki：<https://www.waveshare.com/wiki/ESP32-S3-Matrix>
- TODO：官方 Pinout 图、原理图（存入 `raw/`）
