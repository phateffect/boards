---
type: board
name: M5StickC Plus2
slug: m5stack-stickc-plus2
vendor: M5Stack
aliases: [M5StickC PLUS2, StickC Plus2, StickC+2, SKU K016-P2]
chip: ESP32-PICO-D4
references_modules: []
onboard_modules:
  - ST7789v2 LCD (1.14" 135×240)
  - MPU6886 6-axis IMU
  - BM8563 RTC
  - AXP192 PMU
  - SPM1423 microphone
  - 940nm IR transmitter
  - Red LED
  - Passive buzzer
date_added: 2025-07-21
sources:
  - raw/datasheets/m5stickc-plus_pinmap.txt
tags: [board, esp32, m5stack, wearable, iot]
---

# M5StickC Plus2

> M5 的口香糖形可穿戴开发板，ESP32-PICO-D4 主控，自带 1.14" 屏、六轴 IMU、RTC、麦克风、红外、蜂鸣器。Plus2 是 Plus 的硬件升级版（电池/天线/电源键），**引脚分配与 Plus 完全一致**。

> ⚠️ 引脚数据取自 M5 官方 StickC **Plus** 文档（Plus2 共用同一 ESP32-PICO-D4 与外设布线）。建议把官方引脚图存入 `raw/photos/` 核对。

## 概览

- 厂商 / 型号：M5Stack / M5StickC Plus2（SKU K016-P2）
- 别名：StickC Plus2、StickC+2
- 照片：`![photo](../../raw/photos/m5stack-stickc-plus2.jpg)`（待存）
- 资料：[官方文档（Plus，引脚同）](https://docs.m5stack.com/en/core/m5stickc_plus)

## 主控芯片

- **ESP32-PICO-D4**（SiP，集成 ESP32 + Flash/PSRAM 封装）
  - Xtensa LX6 双核，最高 240MHz
  - Wi-Fi 2.4GHz + BLE
- Flash：4MB（SiP 内置）
- 无外置 PSRAM

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 红色 LED | G10 | GPIO 输出 | 板载指示灯（极性待核实） |
| 红外发射 (940nm) | G9 | IR TX | 红外发射管 |
| 无源蜂鸣器 | G2 | PWM | — |
| 按键（G37 / G39） | G37, G39 | 输入 | 两个用户键；A(主键M5)/B(侧键) 与库的对应关系待核实 |
| 屏幕 ST7789v2 (SPI) | G15 MOSI · G13 CLK · G23 DC · G18 RST · G5 CS | SPI | 1.14" 135×240；背光由 AXP192 供电控制，非独立 GPIO |
| 麦克风 SPM1423 | G0 CLK · G34 DATA | I2S/PDM | G34 仅输入 |
| 内部 I2C 总线 | G21 SDA · G22 SCL | I2C | MPU6886 + BM8563 + AXP192 共用此总线 |

> 电源键（POWER）信号走 AXP192，不占独立 GPIO。

## 引出引脚

| 丝印/接口 | 网络 | 功能 | 备注 |
|---|---|---|---|
| Grove (Port A) SDA | G32 | I2C SDA | 白色 Grove 口 |
| Grove (Port A) SCL | G33 | I2C SCL | 白色 Grove 口 |
| HAT G0 | G0 | GPIO | 与麦克风 CLK 复用，用时注意 |
| HAT G25 | G25 | GPIO/ADC | 与 G36 共用 ADC 通道 |
| HAT G26 | G26 | GPIO | — |
| HAT G36 | G36 | 仅输入/ADC | 与 G25 共用通道，用 G36 时把 G25 设为浮空输入 |
| 5V / 3V3 / GND | — | 电源/地 | Grove 口与 HAT 口均有 |

> HAT 为侧边 0.5mm 间距扩展连接器（接 M5 HAT 配件）。上表 HAT 引脚依据官方"G36/G25 共用通道"说明及 StickC 通用 HAT 定义；**精确排布以官方 HAT 引脚图为准**（建议存图到 `raw/photos/` 核对）。

## 引脚物理排布

```
        ┌───────────────────────┐
  ┌──────┤      1.14" LCD        ├──────┐
  │ M5   │   (135×240 ST7789v2)  │  Btn │
  │ (Btn)└───────────────────────┘  (B) │
  │  USB-C                      Grove   │
  │  (侧边)                     (Port A)│
  │   G0 G25 G26 G36  3V3 5V GND        │
  │   ●  ●   ●   ●    ●   ●  ●  ← HAT   │
  └─────────────────────────────────────┘
  视角：屏幕朝自己、USB-C 在左下；HAT 焊盘在板子底边
  Grove(Port A) = G32(SDA) / G33(SCL) / 5V / GND
```

## 原理图

- [官方 StickC-Plus 原理图 PDF（M5 官网）](https://docs.m5stack.com/en/core/m5stickc_plus) — TODO: 下载到 `raw/schematics/m5stack-stickc-plus2.pdf`

## 示例代码

- [`examples/blink_led/`](examples/blink_led/) — 闪烁板载红色 LED（G10）

## 特殊说明（Plus2 相对 Plus 的差异）

- **引脚分配与 Plus 完全一致**（同 ESP32-PICO-D4、同外设布线）。
- 电池容量升级（Plus 120mAh → Plus2 约 **200mAh**，待核实）。
- 天线设计改进，射频性能更好。
- 电源键行为：长按开机/关机；Plus2 充电与低功耗管理有调整。
- 下载波特率建议：1500000 / 750000 / 500000 / 250000 / 115200 bps（其它可能烧录失败）。
- **G0 / G25 / G26 / G36 为仅输入或共用通道类引脚**，ADC 使用注意 G36↔G25 互斥。
- PMU 仍为 **AXP192**（待核实 Plus2 是否改款）。

## 相关页面

- 同芯片(ESP32) 的板子：[Timer CameraF](../m5stack-timer-cameraf/README.md)
- 暂无引用模组（`references_modules: []`）

## 参考来源

- `raw/datasheets/m5stickc-plus_pinmap.txt`（官方 Plus 页面文本快照，2025-07-21 抓取）
- 官方文档：<https://docs.m5stack.com/en/core/m5stickc_plus>
- TODO：官方原理图 PDF、官方引脚图（存入 `raw/`）
