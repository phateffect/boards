---
type: board
name: ESP32-C3-OLED
slug: esp32-c3-oled
vendor: DIY
aliases: [ESP32-C3 with OLED]
chip: ESP32-C3
references_modules: [ssd1306-oled]
onboard_modules:
  - SSD1306 128×64 OLED (I2C: SCL=GPIO6, SDA=GPIO5)
  - ME6211C33 LDO 3.3V
  - Power LED (红色)
date_added: 2025-07-21
sources:
  - raw/schematics/esp32-c3-oled.pdf
  - raw/c3-oled.ino
tags: [board, esp32-c3, oled, ssd1306, i2c, spi, diy]
---

# ESP32-C3-OLED

> 自制 ESP32-C3 开发板，板载 **SSD1306 128×64 OLED 显示屏**，USB-C 供电/烧录，引出 11 个 GPIO（含 ADC / I2C / SPI / UART），适合小型物联网显示项目。

## 概览

- 厂商 / 型号：自制 / ESP32-C3-OLED
- USB-C 供电 + 烧录
- 板载 SSD1306 OLED（I2C 连接）
- BOOT + RESET 按键

## 主控芯片

- **ESP32-C3**（RISC-V 32-bit 单核，最高 160MHz）
- Flash **4MB**，无 PSRAM
- Wi-Fi 2.4GHz + BLE 5.0
- QFN32 5×5mm 封装

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| SSD1306 OLED | GPIO6(SCL), GPIO5(SDA) | I2C | 板载 OLED 显示屏，详见 [SSD1306 OLED 模组页](../../modules/ssd1306-oled/README.md) |
| ME6211C33 LDO | — | 3.3V 稳压 | USB 5V → 3.3V，Max 300mA（据原理图 C1=4.7μF 输入、C2=4.7μF 输出） |
| Power LED (红色) | VCC 经 R(4.7K) | 电源指示 | 常亮，不可编程 |
| BOOT 按键 | GPIO9 | BOOT / strapping | 按住 BOOT + 按 RESET 进入下载模式 |
| RESET 按键 | EN / CHIP_PU | 复位 | 拉低 EN 复位 |
| USB Type-C | USB D-/D+（GPIO20/GPIO21） | USB | 供电 + 数据/烧录；经 5.1K 电阻上拉至 VUSB |

> ⚠️ 板载 OLED 占用 GPIO5/GPIO6（I2C），如需外部 I2C 设备，可用 GPIO8(SDA)/GPIO9(SCL) 作为第二路 I2C，或避免同时使用板载 OLED 和外部 I2C 设备。

## 引出引脚

> 共 **17 pin** = **11 GPIO** + 6 电源/功能。视角：USB-C 在顶部，正读丝印：

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 5V | 5V | 电源输入 | USB 5V 直通 |
| GND | GND | 地 | — |
| 3V3 | 3V3 | 电源输出 | 经 ME6211C33 稳压，≤300mA |
| RX | GPIO20 | UART0 RX | 串口接收 |
| TX | GPIO21 | UART0 TX | 串口发送 |
| A0 | GPIO0 | ADC1_CH0 | 模拟输入 |
| A1 | GPIO1 | ADC1_CH1 | 模拟输入 |
| A2 | GPIO2 | ADC1_CH2 | 模拟输入 |
| A3 | GPIO3 | ADC1_CH3 / SPI SS | SPI 片选（默认） |
| A4 | GPIO4 | ADC1_CH4 / SPI MOSI | SPI 主出从入 |
| A5 | GPIO5 | ADC1_CH5 / SPI MISO / I2C SDA | ⚠️ 已被板载 OLED 占用（I2C SDA） |
| — | GPIO6 | SPI SS / I2C SCL | ⚠️ 已被板载 OLED 占用（I2C SCL） |
| — | GPIO7 | GPIO | 通用 IO |
| SDA | GPIO8 | I2C SDA（第二路） | 可用于外部 I2C 设备 |
| SCL | GPIO9 | I2C SCL（第二路） | 可用于外部 I2C 设备 |
| — | GPIO10 | GPIO | 通用 IO |

> GPIO0–4 支持 ADC；GPIO3–7 为 SPI 功能组（SS/MOSI/MISO/SS/通用）；GPIO8–9 为 I2C 功能组（第二路）；GPIO20–21 为 UART0。

## 引脚物理排布

```
    视角：正面朝自己 · 丝印正读 · USB-C 在顶部

              ┌────── USB-C ──────┐
          ┌───┴───────────────────┴───┐
          │ ● 5V                 10 ●  │
          │ ● GND                 9 ●  │  SCL (GPIO9)
          │ ● 3V3                 8 ●  │  SDA (GPIO8)
          │ ● RX (GPIO20)         7 ●  │  GPIO7
          │ ● TX (GPIO21)         6 ●  │  (GPIO6, OLED SCL)
          │ ● A0 (GPIO0)          5 ●  │  (GPIO5, OLED SDA)
          │ ● A1 (GPIO1)          4 ●  │  A4 (GPIO4, MOSI)
          │ ● A2 (GPIO2)          3 ●  │  A3 (GPIO3, SS)
          │      [BOOT] [RST]          │
          └────────────────────────────┘
    左列(顶→底): 5V, GND, 3V3, RX, TX, A0, A1, A2
    右列(顶→底): 10, 9(SCL), 8(SDA), 7, 6(OLED), 5(OLED), 4(MOSI), 3(SS)
```

## 原理图

- [原理图 PDF（已入库）](../../raw/schematics/esp32-c3-oled.pdf) — 含 USB Type-C、ME6211C33 LDO、ESP32-C3 QFN32、SSD1306 OLED 模块、BOOT/RESET 按键、Power LED。
- 引脚图（图片）：用户提供，存于对话记录。

## 示例代码

- [`examples/oled_hello_world/`](examples/oled_hello_world/) — 板载 OLED 驱动示例：显示 Hello World + 计数器 + 堆内存监控，使用 Adafruit SSD1306 库。

## 特殊说明

- **GPIO9** 是 ESP32-C3 strapping 引脚，上电电平影响启动模式。做外部 IO 时注意。
- 下载模式：按住 **BOOT** + 按 **RESET**（先松 RESET）。
- 板载 OLED 通过 I2C 连接（SCL=GPIO6, SDA=GPIO5），使用 `Wire.begin(5, 6)` 或 PlatformIO 默认 I2C 引脚。
- USB Type-C 连接 ESP32-C3 原生 USB（GPIO20/GPIO21），无外部 USB-UART 桥。
- 板载有 **两路 I2C**：GPIO5/6（接 OLED）和 GPIO8/9（引出备用），外部 I2C 设备建议用 GPIO8/9。

## 相关页面

- 板载 OLED 模组：[SSD1306 OLED 模组](../../modules/ssd1306-oled/README.md)
- 同芯片(ESP32-C3) 的板子：_(暂无)_

## 参考来源

- `raw/schematics/esp32-c3-oled.pdf`（原理图 PDF，2025-07-21 入库）
- `raw/c3-oled.ino`（原始 Arduino 示例，2025-07-21 入库）
