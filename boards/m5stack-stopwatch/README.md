---
type: board
name: M5Stack StopWatch
slug: m5stack-stopwatch
vendor: M5Stack
aliases: [StopWatch, SKU C152]
chip: ESP32-S3R8
references_modules: []
onboard_modules:
  - CO5300 AMOLED (1.75" 466×466)
  - CST820B capacitive touch
  - BMI270 6-axis IMU
  - RX8130CE RTC
  - ES8311 audio codec + AW8737A amplifier
date_added: 2026-07-27
sources:
  - raw/datasheets/m5stack-stopwatch.pdf
  - raw/schematics/m5stack-stopwatch.pdf
tags: [board, esp32-s3, m5stack, wearable, amoled, touch]
---

# M5Stack StopWatch

> 基于 ESP32-S3R8 的圆形可穿戴开发板，集成 466×466 AMOLED、触摸、IMU、RTC、音频、振动和 450mAh 电池。

## 概览

- 厂商 / 型号：M5Stack StopWatch（SKU C152）
- 尺寸：52 × 52 × 15.5mm；电池：450mAh
- 官方资料：<https://docs.m5stack.com/en/core/StopWatch>

## 主控芯片

- ESP32-S3R8，Xtensa LX7 双核，最高 240MHz。
- 16MB Flash + 8MB PSRAM。

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| CO5300 AMOLED | CS=G39, SCK=G40, TE=G38, D0=G41, D1=G42, D2=G46, D3=G45 | QSPI | 1.75"，466×466；复位由 M5IOE1 PYG5 控制 |
| CST820B 触摸 | SDA=G47, SCL=G48, INT=G13 | I2C | 复位由 M5IOE1 PYG4 控制 |
| BMI270 | SDA=G47, SCL=G48 | I2C | 地址 `0x68` |
| RX8130CE RTC | SDA=G47, SCL=G48 | I2C | 地址 `0x32` |
| ES8311 音频 | SDA=G47, SCL=G48; MCLK=G18, BCLK=G17, ASDOUT=G16, LRCK=G15, DSDIN=G21 | I2C + I2S | AW8737A/扬声器使能经 M5IOE1 |
| 按键 A/B | A=G2, B=G1 | GPIO | — |
| M5IOE1 | SDA=G47, SCL=G48 | I2C GPIO 扩展 | 地址 `0x4F`；还控制振动、屏幕/音频电源 |

## 引出引脚

| 丝印/接口 | 网络 | 功能 | 备注 |
|---|---|---|---|
| Grove 信号 | G10/G11 | GPIO | 接口顺序见下表 |
| Rear G3–G9 | G3–G9 | GPIO | 2.54mm 后部接口 |
| Rear MUX_IO_1 | G43(UART_TX) / G20(USB_DP) | 复用 | 由硬件复用网络决定 |
| Rear MUX_IO_2 | G44(UART_RX) / G19(USB_DM) | 复用 | 由硬件复用网络决定 |
| Rear BOOT | G0 | BOOT/GPIO | strapping |
| 3V3_L2/BAT/GND | 电源网络 | 电源/地 | — |

## 引脚物理排布

后部 2.54mm 2×7：

```text
视角：正对后部插座接触面，按官方 PinMap 的 LEFT/RIGHT 列

 LEFT / 奇数 pin                         RIGHT / 偶数 pin
 ┌──────────────────────────────────────────────────────┐
 │  1 3V3_L2    ●                           ●  2 G3     │
 │  3 MUX_IO_1  ●                           ●  4 G4     │
 │  5 MUX_IO_2  ●                           ●  6 G5     │
 │  7 G9        ●                           ●  8 G6     │
 │  9 BOOT/G0   ●                           ● 10 G7     │
 │ 11 GND       ●                           ● 12 G8     │
 │ 13 NC        ●                           ● 14 BAT    │
 └──────────────────────────────────────────────────────┘
```

```text
Grove / HY2.0-4P（按官方线色与 PinMap 顺序）：
Pin 1 黑=GND | Pin 2 红=5V | Pin 3 黄=G10 | Pin 4 白=G11
```

## 原理图

- [官方原理图](../../raw/schematics/m5stack-stopwatch.pdf)

## 示例代码

- 无；官方 M5Unified 示例可作为起点。

## 特殊说明

- 短按电源键开机/复位，双击关机；长按约 2 秒至绿色 LED 亮起进入下载模式。
- AMOLED、扬声器和振动电机由 M5IOE1 间接控制，不能只按直连 GPIO 初始化。

## 相关页面

- 同芯片系列：[StickS3](../m5stack-sticks3/README.md)

## 参考来源

- [官方产品页归档](../../raw/datasheets/m5stack-stopwatch.pdf)
- [官方原理图](../../raw/schematics/m5stack-stopwatch.pdf)
- 官方产品页：<https://docs.m5stack.com/en/core/StopWatch>
