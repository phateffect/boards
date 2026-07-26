---
type: module
name: Osptek YDP420H003-V3 4.2" BWR TFT
slug: osptek-ydp420h003-v3
vendor: Osptek
aliases: [Osptek 4.2 BWR TFT, YDP420H003-V3]
device: ST7306
interface: [SPI]
date_added: 2026-07-27
used_by: []
sources:
  - raw/opstek-4.2-bwr-tft.pdf
  - raw/opstek-4.2-hat.pdf
  - raw/reference-code/osptek-4.2-bwr-tft-driver
tags: [module, display, tft, reflective, bwr, st7306, spi]
---

# Osptek YDP420H003-V3 4.2" BWR TFT

> 300×400 黑/白/红反射式 TFT 屏体，使用 ST7306 和四线 SPI；本页同时记录 24-pin FPC 与 8-pin 转接板。

## 概览

- 厂商 / 型号：Osptek YDP420H003-V3
- 4.2"，300×400，黑/白/红，反射式、无背光
- 有效区 63.6 × 84.8mm；模组 67.6 × 91.0 × 1.1mm；约 14g
- 工作温度 -20–70°C；存储温度 -30–80°C

## 核心器件

- 驱动 IC：ST7306。
- 通信：4-line SPI，信号为 SCLK、SDI、CS、D/C，另有 RES 与 TE。
- 屏体 IOVCC 为 1.65–3.3V，VCI 为 2.55–3.3V。

## 引脚定义

### 8-pin 转接板

| Pin | 丝印/网络 | 功能 | 典型接法 |
|---|---|---|---|
| 1 | VCI | 电源 | 3.3V |
| 2 | GND | 地 | GND |
| 3 | SCLK | SPI 时钟 | 任意可映射 SPI SCK |
| 4 | SDI | SPI 数据输入 | 任意可映射 SPI MOSI |
| 5 | RES | 复位 | GPIO |
| 6 | DC | 数据/命令选择 | GPIO |
| 7 | CS | SPI 片选 | GPIO |
| 8 | TE | Tearing Effect 同步 | GPIO 输入，可选 |

### 屏体 24-pin FPC

| Pin | 网络 | 说明 |
|---|---|---|
| 1–4 | NC | 不连接 |
| 5 | VCC(I-O) | 规格书原文命名；与 Pin15 IOVCC 的关系以转接板原理图为准 |
| 6–8 | NC | 不连接 |
| 9 | TE | 同步输出 |
| 10 | LCD_RES | 复位 |
| 11 | LCD_D/C | 数据/命令 |
| 12 | LCD_CS | 片选 |
| 13 | LCD_SCLK | SPI 时钟 |
| 14 | LCD_SDI | SPI 数据 |
| 15 | IOVCC | 1.65–3.3V |
| 16 | VCI | 2.55–3.3V |
| 17 | GND | 地 |
| 18 | VGH | 内部电源输出 |
| 19 | NC | 不连接 |
| 20 | VIV | 内部电源输出（沿用规格书网络名） |
| 21 | AVDD | 内部电源输出 |
| 22 | VNV | 内部电源输出 |
| 23 | NC | 不连接 |
| 24 | VGL | 内部电源输出 |

## 接口

- SPI mode 0，MSB first；参考代码低功耗模式使用 10MHz，高速模式使用 20MHz。
- 像素以 4-bit nibble 编码：neutral=`0x0`、red=`0x3`、white=`0xC`、black=`0xF`。
- 参考驱动使用 30,000-byte framebuffer，并按 ST7306 的 2:1 交错扫描以 300×200 逻辑画布映射到 300×400 面板。
- 默认 1Hz 低功耗模式；16Hz 高速配置来自第三方参考代码，属于实验性路径，应先在实屏验证功耗和显示稳定性。

## 引脚物理排布

转接板 1×8 排针按原理图 Pin1→Pin8：

```text
1 VCI | 2 GND | 3 SCLK | 4 SDI | 5 RES | 6 DC | 7 CS | 8 TE
```

转接板以 `R1=0Ω` 把 IOVCC 与 VCI 相连，默认应统一使用 **3.3V**。只有需要独立 1.8V I/O 时才移除 R1，并分别满足 IOVCC/VCI 的电压范围。

## 示例代码

- [`examples/st7306_basic/`](examples/st7306_basic/) — 从第三方 MIT 参考驱动提炼的 PlatformIO 示例：引脚可配置、初始化 ST7306、维护 300×200 逻辑 framebuffer，并显示黑/白/红测试图。

## 被哪些开发板使用

_(无；这是独立 SPI 模组，示例不绑定 ESP32-S3 SuperMini。)_

## 相关页面 / 参考来源

- [屏体规格书](../../raw/opstek-4.2-bwr-tft.pdf)
- [8-pin 转接板原理图](../../raw/opstek-4.2-hat.pdf)
- [第三方参考驱动归档](../../raw/reference-code/osptek-4.2-bwr-tft-driver/ORIGIN.md)（MIT）
- 上游参考代码：<https://github.com/CaiZiYuan2019/Osptek-4.2-BWR-TFT-Driver-for-ESP32-S3>
