---
type: board
name: M5StickC Plus2
slug: m5stack-stickc-plus2
vendor: M5Stack
aliases: [M5StickC PLUS2, StickC Plus2, StickC+2, SKU K016-P2]
chip: ESP32-PICO-V3-02
references_modules: []
onboard_modules:
  - ST7789V2 LCD (1.14" 135×240)
  - MPU6886 6-axis IMU (I2C 0x68)
  - BM8563 RTC
  - SPM1423 microphone
  - IR emitter (940nm)
  - Red LED + Green LED (sleep indicator)
  - Passive buzzer
  - Power: SY8089(3V3) + SY7088(5V) + TP4057(charger) — 无 PMIC（去掉了 AXP192）
date_added: 2025-07-21
sources:
  - raw/datasheets/m5stickc-plus2_pinmap.txt
  - raw/schematics/m5stack-stickc-plus2.pdf
  - raw/assets/m5stack-stickc-plus2-pin-layout.webp
tags: [board, esp32, m5stack, wearable, iot]
---

# M5StickC Plus2

> M5 口香糖形可穿戴开发板。**Plus2 ≠ Plus**：主控换成 **ESP32-PICO-V3-02（带 2MB PSRAM）**，**去掉了 AXP192**，电池升到 200mAh，红色 LED 与红外发射**共用 G19**，TFT 的 DC/RST 引脚也变了。引脚数据据官方 Plus2 文档 + 原理图 v0.5 核实。

## 概览

- 厂商 / 型号：M5Stack / M5StickC Plus2（SKU K016-P2）
- 别名：StickC Plus2、StickC+2
- 资料：<https://docs.m5stack.com/en/core/M5StickC%20PLUS2>
- 原理图：[`raw/schematics/m5stack-stickc-plus2.pdf`](../../raw/schematics/m5stack-stickc-plus2.pdf)（v0.5）

## 主控芯片

- **ESP32-PICO-V3-02**（SiP 模组，Xtensa LX6 双核，最高 240MHz，Wi-Fi + BLE）
- **8MB Flash + 2MB PSRAM**（模组内置；这是相对 Plus 的重大升级——Plus 是 PICO-D4，4MB Flash、无 PSRAM）
- RTC SRAM 16KB

> ⚠️ 与 Plus 的关键差异见下文「特殊说明」。如果你的代码按 Plus 写，引脚/电源都要改。

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 红色 LED + 红外发射 (940nm) | **G19**（共用） | GPIO/RMT | 红色 LED 与 IR 发射管**共用同一个控制脚 G19**；另有**绿色 LED 不可编程**（睡眠指示） |
| 按键 A（M5 主键） | G37 | 输入，active-low | 10K 上拉；G37=SENSOR_CAPP |
| 按键 B（侧键） | G39 | 输入，active-low | 仅输入（SENSOR_VN） |
| 按键 C（电源/WAKE） | G35 | 输入/唤醒 | 长按 >2s 开机、>6s 关机；仅输入 |
| 无源蜂鸣器 | G2 | PWM | 经 Q2（SS8050）驱动 |
| 屏幕 ST7789V2 (SPI) | MOSI=G15 · CLK=G13 · DC=**G14** · RST=**G12** · CS=G5 · BL=**G27** | SPI | 1.14" 135×240；**背光=G27**（GPIO，不再走 AXP192） |
| 麦克风 SPM1423 | G0 CLK · G34 DATA | I2S/PDM | G34 仅输入 |
| 内部 I2C（MPU6886 + BM8563） | G21 SDA · G22 SCL | I2C | MPU6886 地址 0x68 |
| HOLD（电源保持） | G4 | 输出 | 开机后程序须置 G4=1 保持供电；置 0 关机 |
| 电池电压检测 | G38 | ADC（仅输入） | 无 AXP192，用 ADC 直接测电池电压 |
| USB-UART (CH9102) | G43 TX0 · G44 RX0 | UART0 | 下载/调试；驱动为 CP34X(CH9102) |
| 电源管理（无 PMIC） | — | — | SY8089(3V3 降压) + SY7088(5V 升压) + TP4057(充电) + BM8563(RTC)；**AXP192 已移除** |

## 引出引脚

| 丝印/接口 | 网络 | 功能 | 备注 |
|---|---|---|---|
| Grove / HY2.0-4P (PORT.CUSTOM) SDA | G32 | I2C SDA | Grove 口（I2C） |
| Grove / HY2.0-4P (PORT.CUSTOM) SCL | G33 | I2C SCL | |
| HAT G0 | G0 | GPIO | 与麦克风 CLK 复用 |
| HAT G25 | G25 | GPIO/ADC | |
| HAT G26 | G26 | GPIO | |
| HAT G36 | G36 | 仅输入/ADC | |
| 5V / 3V3 / GND | — | 电源/地 | Grove 与 HAT 口均有 |

> HAT 为侧边扩展连接器（接 M5 HAT 配件）。上表 HAT 脚位沿用 StickC 系列约定（官方 Plus2 文档强调 HAT/Unit 兼容）；精确排布以官方 HAT 引脚图为准。

## 引脚物理排布

```text
正面视角：屏幕朝自己、USB-C 在下

        ┌────────────────────────┐
        │      1.14" LCD         │
 Btn B  │                        │  Btn A
        │                        │
        │ IR/LED  MIC  BUZZER    │
        │ Btn C/PWR       USB-C  │
        └────────────────────────┘

背面视角：背面丝印正读、USB-C 与 Grove 在下

  HAT 8-pin（自左向右）：
  ┌────┬─────┬─────┬─────────┬────┬─────┬─────┬─────┐
  │GND │5V ↑ │ G26 │ G36/G25 │ G0 │ BAT │ 3V3 │5V ↓ │
  └────┴─────┴─────┴─────────┴────┴─────┴─────┴─────┘
                          ┌──────────────┐
  Grove / HY2.0-4P（下端）│ GND  5V G32 G33 │
                          └──────────────┘
```

- `G36/G25` 是官方背面丝印在**同一个 HAT 物理位**上的写法，不应拆成两个插针。
- HAT 两个 5V 位以丝印箭头区分方向（`5V ↑` / `5V ↓`）；使用前按具体 HAT 的供电方向核对。
- Grove/PORT.CUSTOM：G32=SDA、G33=SCL。
- 红 LED 与 IR 共用 G19；电源键 C=G35；HOLD=G4；电池检测=G38。

## 原理图

- [原理图 PDF v0.5（已入库）](../../raw/schematics/m5stack-stickc-plus2.pdf) — 3 页：主控+外设 / 电源 / 传感器。本页占用引脚表据此 + 官方文档交叉核实。
- [官方背面接口图](../../raw/assets/m5stack-stickc-plus2-pin-layout.webp) — 页面已将其中全部脚位转录为 ASCII。

## 示例代码

- [`examples/blink_led/`](examples/blink_led/) — 闪烁板载红色 LED。**注意：红 LED 在 G19，与 IR 共用**（toggle G19 会同时驱动 IR，肉眼只见红 LED）。

## 特殊说明（Plus2 相对 Plus 的关键差异）

| 项 | Plus | **Plus2** |
|---|---|---|
| 主控 | ESP32-PICO-D4 | **ESP32-PICO-V3-02**（带 2MB PSRAM） |
| Flash | 4MB | **8MB** |
| PSRAM | 无 | **2MB** |
| 电源管理 | AXP192 | **无 PMIC**（SY8089+SY7088+TP4057+BM8563） |
| 电池 | 120mAh | **200mAh @3.7V** |
| USB-UART | CH522 | **CH9102**（CP34X 驱动） |
| 红色 LED | G10 | **G19（与 IR 共用）** |
| IR | G9 | **G19** |
| TFT DC / RST | G23 / G18 | **G14 / G12** |
| TFT 背光 | 经 AXP192 | **G27（GPIO）** |
| 按键 A / B | G37 / G39 | G37 / G39（不变） |
| 按键 C | 普通复位键 | **G35（WAKE/电源键）** |
| HOLD | — | **G4**（开机后须置 1） |
| 电池电压检测 | 经 AXP192 | **G38（ADC）** |

**开关机（无 AXP192 的特殊逻辑）：**
- 开机：按 BUTTON C(G35) >2s，或经 BM8563 RTC IRQ 唤醒；**唤醒后程序必须置 HOLD(G4)=1**，否则会再次关机。
- 关机（未接 USB）：按 BUTTON C >6s，或程序置 HOLD(G4)=0。
- 接 USB 时按 BUTTON C >6s：关屏进入睡眠（非完全关机）。
- 烧录波特率建议 1500000 / 750000 / 500000 / 250000 / 115200 bps。

## 相关页面

- 同芯片系列(ESP32) 的板子：[Timer Camera F](../m5stack-timer-cameraf/README.md)
- 暂无引用模组（`references_modules: []`）

## 参考来源

- `raw/datasheets/m5stickc-plus2_pinmap.txt`（官方 Plus2 文档文本快照，2025-07-21）
- `raw/schematics/m5stack-stickc-plus2.pdf`（官方原理图 v0.5，2025-07-21 入库）
- `raw/assets/m5stack-stickc-plus2-pin-layout.webp`（官方背面接口图，2026-07-27 入库）
- 官方文档：<https://docs.m5stack.com/en/core/M5StickC%20PLUS2>
