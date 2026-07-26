---
type: board
name: M5Stack StickS3
slug: m5stack-sticks3
vendor: M5Stack
aliases: [StickS3, M5StickS3, SKU K150]
chip: ESP32-S3-PICO-1-N8R8
references_modules: []
onboard_modules:
  - ST7789P3 LCD (1.14" 135×240)
  - BMI270 6-axis IMU (I2C 0x68)
  - ES8311 audio codec + AW8737 amplifier
  - M5PM1 power management (I2C 0x6E)
  - IR transmitter and receiver
date_added: 2026-07-27
sources:
  - raw/datasheets/m5stack-sticks3.pdf
  - raw/schematics/m5stack-sticks3.pdf
tags: [board, esp32-s3, m5stack, wearable, lcd]
---

# M5Stack StickS3

> 基于 ESP32-S3-PICO-1-N8R8 的口香糖形开发板，集成 1.14" LCD、IMU、音频、红外和 250mAh 电池。

## 概览

- 厂商 / 型号：M5Stack StickS3（SKU K150）
- 主控：ESP32-S3-PICO-1-N8R8，8MB Flash + 8MB Octal PSRAM
- 尺寸：48 × 24 × 15mm；电池：250mAh
- 官方资料：<https://docs.m5stack.com/en/core/StickS3>

## 主控芯片

- Xtensa LX7 双核，最高 240MHz，支持 Wi-Fi 与 Bluetooth LE。
- 模组集成 8MB Flash 和 8MB Octal PSRAM。

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| ST7789P3 LCD | MOSI=G39, SCK=G40, DC=G45, CS=G41, RST=G21, BL=G38 | SPI | 135×240 |
| BMI270 | SDA=G47, SCL=G48 | I2C | 地址 `0x68` |
| M5PM1 | SDA=G47, SCL=G48 | I2C | 地址 `0x6E` |
| ES8311 | SDA=G47, SCL=G48; MCLK=G18, DOUT=G14, BCLK=G17, LRCK=G15, DIN=G16 | I2C + I2S | 地址 `0x18`，配 AW8737 与 8Ω/1W 扬声器 |
| 按键 | KEY1=G11, KEY2=G12 | GPIO | — |
| 红外 | TX=G46, RX=G42 | RMT/GPIO | 发射与接收 |

## 引出引脚

| 丝印/接口 | 网络 | 功能 | 备注 |
|---|---|---|---|
| Grove 黄线 | G9 | GPIO / 可作 SDA | Grove 口信号 |
| Grove 白线 | G10 | GPIO / 可作 SCL | Grove 口信号 |
| Grove 5V/GND | 5V/GND | 电源/地 | — |
| HAT2 G1–G8 | G1–G8 | GPIO | 见下方排布 |
| HAT2 G43/G44 | G43/G44 | UART 等 | — |
| HAT2 BOOT | BOOT | 启动配置 | 上电勿误拉低 |
| HAT2 EXT_5V | EXT_5V | 可控 5V | 默认不输出 |
| HAT2 3V3_L2 | 3V3_L2 | 3.3V 电源 | 扩展电源 |
| HAT2 5V_IN/BAT/GND | 5V_IN/BAT/GND | 电源/地 | 注意方向与电压 |

## 引脚物理排布

HAT2 16-pin（按官方 PinMap 编号）：

```text
Pin 1  GND       Pin 2  G5
Pin 3  EXT_5V    Pin 4  G4
Pin 5  BOOT      Pin 6  G6
Pin 7  G1        Pin 8  G7
Pin 9  G8        Pin 10 G43
Pin 11 BAT       Pin 12 G44
Pin 13 3V3_L2    Pin 14 G2
Pin 15 5V_IN     Pin 16 G3
```

| 接口 | 类型 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|---|
| Grove | HY2.0-4P | GND | 5V | G9（黄） | G10（白） |

## 原理图

- [官方原理图](../../raw/schematics/m5stack-sticks3.pdf)

## 示例代码

- 无；官方 M5Unified 示例可作为起点。

## 特殊说明

- 下载模式：长按侧面复位键，直到绿色 LED 闪烁。
- `EXT_5V` 默认关闭/作为输入；需要输出时可用 `M5.Power.setExtOutput(true)`。不得把外部 5V 输入与受控 5V 输出错误并接。
- Mini JoyC（U156）、Mini EncoderC（U157）和 Hat 18650C（U080）存在机械不兼容。

## 相关页面

- 同系列：[M5StickC Plus2](../m5stack-stickc-plus2/README.md)
- 可通过 HAT2 或改线连接：[RoverC-Pro](../../modules/m5stack-roverc-pro/README.md)

## 参考来源

- [官方产品页归档](../../raw/datasheets/m5stack-sticks3.pdf)
- [官方原理图](../../raw/schematics/m5stack-sticks3.pdf)
- 官方产品页：<https://docs.m5stack.com/en/core/StickS3>
