---
type: board
name: ESP32-S3 SuperMini
slug: esp32-s3-supermini
vendor: generic
aliases: [ESP32S3 SuperMini, ESP32-S3 Super Mini]
chip: ESP32-S3FH4R2
references_modules: []
onboard_modules:
  - WS2812 RGB LED (GPIO48)
  - Red LED (GPIO48)
  - TP4054 Li-ion charger
date_added: 2026-07-27
sources:
  - raw/datasheets/esp32-s3-supermini-nologo.pdf
  - raw/schematics/esp32-s3-supermini.png
  - raw/assets/esp32-s3-supermini-pinout.jpg
  - raw/assets/esp32-s3-supermini-dimension.jpg
tags: [board, esp32-s3, supermini, usb, battery]
---

# ESP32-S3 SuperMini

> 小尺寸 ESP32-S3FH4R2 开发板，带原生 USB、4MB Flash、2MB PSRAM、RGB LED 和单节锂电充电电路。

## 概览

- 型号：ESP32-S3 SuperMini（通用板型）
- 参考资料：<https://wiki.nologo.tech/product/esp32/esp32s3/esp32s3supermini/esp32S3SuperMini.html>
- 引脚图：![ESP32-S3 SuperMini 引脚图](../../raw/assets/esp32-s3-supermini-pinout.jpg)

## 主控芯片

- ESP32-S3FH4R2：Xtensa LX7 双核，最高 240MHz。
- 芯片封装内含 4MB Flash 与 2MB PSRAM。

> 参考网页把架构/频率写成“RISC-V 单核 160MHz”，与 Espressif 的 ESP32-S3 规格冲突；本页按芯片官方规格订正。

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 原生 USB | G19(D-), G20(D+) | USB 2.0 OTG / Serial-JTAG | USB-C 直连芯片 |
| WS2812 RGB LED | G48 | 单线 RGB | 与红色 LED 共用 GPIO |
| 红色 LED | G48 | GPIO | 与 WS2812 数据脚共用，使用时会互相影响 |
| BOOT 按键 | G0 | 启动配置 | 按住复位可进下载模式 |
| RESET 按键 | EN | 复位 | — |
| TP4054 | B+/B- | 单节锂电充电 | BOOST 焊桥改变充电电流 |

## 引出引脚

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| TX/RX | U0TXD/U0RXD | UART0 | — |
| 0 | G0 | BOOT/GPIO | strapping |
| 1–18 | G1–G18 | GPIO/外设复用 | 具体位置见下图 |
| 21 | G21 | GPIO | — |
| 33–42 | G33–G42 | GPIO/外设复用 | 背面焊盘 |
| 45–48 | G45–G48 | GPIO/strapping | G48 与两个 LED 共用 |
| VBUS/3V3/GND | 电源网络 | 电源/地 | 3V3 由 ME6217C33 提供 |
| B+/B- | 电池正/负 | 单节锂电池 | 注意极性 |

## 引脚物理排布

正面两侧排针（从 USB-C 端向下看，按资料图自上而下）：

```text
左：TX, RX, G1, G2, G3, G4, G5, G6, G7
右：VBUS, GND, 3V3, G13, G12, G11, G10, G9, G8
```

背面焊盘：

```text
左：G38, G37, G36, G35, G34, G33, G18, G17, G14, G15, G16, G21
右：G48, G47, G46, G45, G42, G41, G40, G39
```

## 原理图

- [参考原理图](../../raw/schematics/esp32-s3-supermini.png)
- [尺寸图](../../raw/assets/esp32-s3-supermini-dimension.jpg)

## 示例代码

- 无。

## 特殊说明

- G48 同时连接 WS2812 数据输入与红色 LED；驱动其中一个时另一个可能闪烁或误响应。
- `BOOST` 焊桥据资料会把充电电流从约 100mA 提高到约 300mA；仅应给容量大于 500mAh、允许该充电倍率的电池使用。
- 电池接口按单节锂离子/锂聚合物系统理解。不要把网页的宽泛电压描述当作可直接接入 6V 的依据。

## 相关页面

- 同芯片系列：[StickS3](../m5stack-sticks3/README.md)、[StopWatch](../m5stack-stopwatch/README.md)、[Waveshare ESP32-S3-Matrix](../waveshare-esp32-s3-matrix/README.md)

## 参考来源

- [参考网页归档](../../raw/datasheets/esp32-s3-supermini-nologo.pdf)
- [原理图](../../raw/schematics/esp32-s3-supermini.png)
- [引脚图](../../raw/assets/esp32-s3-supermini-pinout.jpg)
- [尺寸图](../../raw/assets/esp32-s3-supermini-dimension.jpg)
- 外部参考：<https://wiki.nologo.tech/product/esp32/esp32s3/esp32s3supermini/esp32S3SuperMini.html>
