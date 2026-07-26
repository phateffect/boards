---
type: board
name: ESP32-S3-AI-Board
slug: esp32-s3-ai-board
vendor: OpenJumper
aliases: [ESP32S3AI board, ESP32-S3 AI Board, ESP32-S3-AIKIT]
chip: ESP32-S3-WROOM-1-N16R8
references_modules: []
onboard_modules:
  - ST7789 TFT (1.9" 170×320)
  - Digital I2S microphone
  - I2S audio decoder/amplifier
  - WS2812B-2020 RGB LED
  - TP4054 Li-ion charger
date_added: 2026-07-27
sources:
  - raw/datasheets/esp32-s3-ai-board-arduino-me.pdf
  - raw/schematics/esp32-s3-ai-board.webp
  - raw/assets/esp32-s3-ai-board-pinout-front.webp
  - raw/assets/esp32-s3-ai-board-pinout-back.webp
  - raw/assets/esp32-s3-ai-board-layout-annotated.webp
tags: [board, esp32-s3, ai, lcd, audio, microphone, battery]
---

# ESP32-S3-AI-Board

> OpenJumper 面向语音交互和 AI 应用的 ESP32-S3 开发板，集成 1.9" LCD、数字麦克风、I2S 音频功放、扬声器接口、RGB LED 与锂电供电。

## 概览

- 型号：ESP32-S3-AI-Board / ESP32-S3-AIKIT
- 主控模组：ESP32-S3-WROOM-1-N16R8
- 尺寸：63.5 × 33mm
- 供电：USB-C 5V 或 MX1.25-2P 单节 3.7V 锂电池；板载 TP4054 充电
- 资料：<https://arduino.me/a/3274>

![正面引脚图](../../raw/assets/esp32-s3-ai-board-pinout-front.webp)

> 图片只作来源佐证；完整排针和连接器顺序已转录到下方 ASCII 图。

## 主控芯片

- ESP32-S3-WROOM-1-N16R8：Xtensa LX7 双核，最高 240MHz。
- 16MB Quad Flash + 8MB Octal PSRAM，支持 Wi-Fi 与 Bluetooth LE。
- USB-C 的 D-/D+ 直接连接 G19/G20，使用 ESP32-S3 原生 USB。

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| ST7789 TFT | DC=G11, MOSI/SDA=G10, SCLK=G12, CS=G13, RST=G14, BL=G3 | SPI | 1.9"，170×320；参考例程 `setRotation(3)` |
| 数字麦克风 | SCK=G5, SD=G6, WS/LRCLK=G4 | I2S 输入 | 原理图未标具体型号；L/R 选择脚上拉至 3.3V |
| 音频解码/功放 | DIN=G7, BCLK=G15, LRCLK=G16 | I2S 输出 | 原理图未标具体型号；差分输出接 SPK 插座 |
| WS2812B-2020 | DIN=G48 | 单线 RGB | G48 同时引出到排针 |
| VOL- / VOL+ | G39 / G40 | 按键输入 | 按下接 GND，active-low |
| BOOT | G0 | 按键输入/strapping | 按下接 GND，也作为“对话打断”键 |
| RESET | EN | 复位 | 按下接 GND |
| 原生 USB | D-=G19, D+=G20 | USB Serial/JTAG/OTG | 无外部 USB-UART 桥 |
| 充电 | TP4054 | 单节锂电充电 | 充电管理未连接到主控 GPIO |

## 引出引脚

### 两侧 2×16 排针

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 3V3 ×2 | 3V3 | 3.3V 电源 | — |
| EN | EN | 复位/芯片使能 | active-low |
| 0 | G0 | BOOT/GPIO | strapping、BOOT 按键共网 |
| 1, 2, 8, 9, 17, 18, 21, 35–42, 45–48 | 对应 GPIO | GPIO/外设复用 | G39/G40/G48 已被板载器件复用 |
| TX/RX | G43/G44 | UART0 TX/RX | 原理图确认 |
| V | VIN | 电源输入网络 | 电源开关后的 VIN |
| G | GND | 地 | 多个位置 |

### 独立连接器

| 丝印 | 类型 | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
|---|---|---|---|---|---|
| I2C | MX1.25-4P | SCL (G42) | SDA (G41) | GND | VIN |
| UART | MX1.25-4P | RX | TX | GND | VIN |
| BAT | MX1.25-2P | BAT+ | GND | — | — |
| SPK | 2-pin | OUTN | OUTP | — | — |

> I2C/UART 插座的 Pin 1→4 顺序来自原理图连接器编号；从板背面看，板上丝印自上而下分别写作 I2C=`42, 41, G, V`、UART=`V, G, TX, RX`，后者与 Pin 编号方向相反。

## 引脚物理排布

### 两侧排针

```text
视角：元件/屏幕面朝上，屏幕在上，USB-C 和按键在下
每行左右焊盘处于同一高度；自上而下。

                  ┌──────── 1.9" LCD ────────┐
        GND   ● ──┤                          ├── ● 3V3
         G1   ● ──┤                          ├── ● 3V3
         G2   ● ──┤                          ├── ● EN
    TX / G43  ● ──┤                          ├── ● G17
    RX / G44  ● ──┤                          ├── ● G18
         G42  ● ──┤                          ├── ● G8
         G41  ● ──┤                          ├── ● G19 / USB D-
         G40  ● ──┤                          ├── ● G20 / USB D+
         G39  ● ──┤                          ├── ● G46
         G38  ● ──┤                          ├── ● G9
         G37  ● ──┤                          ├── ● G21
         G36  ● ──┤                          ├── ● G47
         G35  ● ──┤                          ├── ● G48 / WS2812
    BOOT / G0 ● ──┤                          ├── ● G45
        GND   ● ──┤  VOL-  MIC  RGB  VOL+   ├── ● VIN
        GND   ● ──┤       USB-C / RST/BOOT   ├── ● GND
                  └──────────────────────────┘
```

产品页背面示意图把 G42 位置误画成第二个 G40；正面图与原理图均为 `G42, G41, G40, G39`，本页采用两者一致的结果。

### 背面连接器

```text
视角：板背面朝上，天线在上、USB-C 在下

左侧 SPK：  OUTP | OUTN               （2-pin）
左侧 I2C：  42/SCL | 41/SDA | GND | VIN（丝印自上而下）
左下 BAT：  BAT+ | GND                 （以原理图 Pin1→Pin2）
右侧 UART： VIN | GND | TX | RX        （丝印自上而下）
底部中央：  USB-C
底部右侧：  RST | BOOT                 （从左到右）
```

## 原理图

- [单页原理图](../../raw/schematics/esp32-s3-ai-board.webp)（ESP32-S3-AIKIT v0.1，更新日期 2025-02-05）
- [背面接口图](../../raw/assets/esp32-s3-ai-board-pinout-back.webp)

## 示例代码

- [`examples/board_self_test/`](examples/board_self_test/) — PlatformIO 分阶段自检：LCD、WS2812、三个按键、I2S 麦克风电平和 I2S 扬声器测试音。各阶段可用 build flag 单独关闭。

## 特殊说明

- BOOT/G0 是 strapping 引脚；外部电路不要在上电时强制错误电平。
- G39、G40、G48 虽引出到排针，但分别与音量按键和 WS2812 共网。
- I2C 与 UART 插座电源脚是 **VIN**，不是 3.3V；连接只接受 3.3V 的外设时不能直接把 VIN 当逻辑电源。
- 产品页只写“数字麦克风”和“音频解码功放”，原理图也未给 U4/U5 型号。本页记录已确认的 I2S 接线，不猜测具体器件。
- 电源开关必须打开才能正常烧录/运行。USB 为原生 USB，PlatformIO 示例启用 USB CDC on boot。

## 相关页面

- 同芯片系列：[ESP32-S3 SuperMini](../esp32-s3-supermini/README.md)、[M5Stack StickS3](../m5stack-sticks3/README.md)、[M5Stack StopWatch](../m5stack-stopwatch/README.md)、[Waveshare ESP32-S3-Matrix](../waveshare-esp32-s3-matrix/README.md)

## 参考来源

- [产品页 PDF 归档](../../raw/datasheets/esp32-s3-ai-board-arduino-me.pdf)
- [原理图](../../raw/schematics/esp32-s3-ai-board.webp)
- [正面引脚图](../../raw/assets/esp32-s3-ai-board-pinout-front.webp)
- [背面引脚图](../../raw/assets/esp32-s3-ai-board-pinout-back.webp)
- [标注实物布局](../../raw/assets/esp32-s3-ai-board-layout-annotated.webp)
- 原始产品页：<https://arduino.me/a/3274>
