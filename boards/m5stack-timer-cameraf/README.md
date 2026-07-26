---
type: board
name: M5Stack Timer Camera F
slug: m5stack-timer-cameraf
vendor: M5Stack
aliases: [TimerCam F, TimerCamera-F, SKU U082-F]
chip: ESP32-D0WDQ6-V3
references_modules:
  - ov3660-camera
onboard_modules:
  - OV3660 camera (120° fisheye)   # → 见 modules/ov3660-camera，本页不重复器件细节
  - BM8563 RTC
  - Status LED
  - HY2.0-4P expansion port
date_added: 2025-07-21
sources:
  - raw/datasheets/m5stack-timercam-f_pinmap.txt
  - raw/datasheets/m5stack-timercam-f_library-power.txt
  - raw/assets/m5stack-timer-cameraf-port-layout.jpg
tags: [board, esp32, m5stack, camera, low-power, combo]
---

# M5Stack Timer Camera F

> ESP32 超低功耗摄像头板:**ESP32-D0WDQ6-V3 + 8MB PSRAM**,板载 **OV3660**(3MP、120° 鱼眼)+ BM8563 RTC,靠 BM8563 定时闹钟做超低功耗定时拍照（IRQ 触发电源电路重新上电，非 GPIO 唤醒），待机电流可低至 ~2μA,内置 270mAh 电池可撑一个多月(每小时一拍)。

> 📌 **核心板 + 模组组合体**：摄像头部分 = [OV3660 摄像头模组](../../modules/ov3660-camera/README.md)。本页只说明「这颗 OV3660 在本板上接到哪些 GPIO」（板级接线），**不重复 OV3660 的器件细节**——点过去看。

> ⚠️ 引脚数据取自 M5 官方 TimerCamera-F 文档（2025-07-21）。建议把官方原理图/引脚图存入 `raw/` 复核。

## 概览

- 厂商 / 型号：M5Stack / Timer Camera F（SKU U082-F）
- 资料：<https://docs.m5stack.com/en/unit/timercam_f>
- 定位：定时拍照 / 远程监控 / WiFi 图传

## 主控芯片

- **ESP32-D0WDQ6-V3**（Xtensa LX6 双核，240MHz，Wi-Fi + BLE）
- **8MB PSRAM**（拍照缓冲必备）
- **4MB Flash**

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| **摄像头 OV3660** | 见下方「摄像头 DVP 接线」表 | DVP 并行摄像头 | 器件细节见 [OV3660 模组页](../../modules/ov3660-camera/README.md) |
| RTC BM8563 | SDA=G12 · SCL=G14 · INT(pin3)→`RTC_ALM` | I2C | INT(IRQ)**不接 ESP32 GPIO**——直接进电源开关电路；定时到点经它重新上电（见「唤醒机制」） |
| 状态 LED | G2 | GPIO 输出 | 指示灯 |
| 电池检测/保持 | G38 BAT_ADC · G33 POWER_HOLD | ADC1_CH2 / 控制 | 库核实：开机须置 HOLD(G33)=1 维持供电，置 0 关机；内置 270mAh 锂电池 |
| PWR 键 | —（电源管理） | 电源开关 | 长按 2s 开机；关机用软件 API 或板载 RESET 键 |
| RESET 键 | EN/CHIP_PU | 复位 | 板载微动 |

> 电源管理采用与 CORE/StickC **不同**的低功耗方案（非 AXP192）。

### 摄像头 DVP 接线（板级，OV3660 ↔ ESP32）

> 这是本板把 OV3660 的各信号接到 ESP32 的哪根 GPIO。OV3660 各信号含义见 [OV3660 模组页](../../modules/ov3660-camera/README.md#引脚定义传感器信号)。

| OV3660 信号 | 含义 | TimerCam GPIO |
|---|---|---|
| SIOC | SCCB 时钟 | **G23** |
| SIOD | SCCB 数据 | **G25** |
| XCLK | 外部主时钟 | **G27** |
| VSYNC | 帧同步 | **G22** |
| HREF | 行有效 | **G26** |
| PCLK | 像素时钟 | **G21** |
| D0 | 数据位0 | **G32** |
| D1 | 数据位1 | **G35**（仅输入） |
| D2 | 数据位2 | **G34**（仅输入） |
| D3 | 数据位3 | **G5** |
| D4 | 数据位4 | **G39**（仅输入） |
| D5 | 数据位5 | **G18** |
| D6 | 数据位6 | **G36**（仅输入） |
| D7 | 数据位7 | **G19** |
| RESET# | 复位 | **G15** |
| PWDN | 掉电 | **-1**（未接） |
| 3V3 / GND | 供电 | 电源 |

> 摄像头吃掉了约 15 个 GPIO，可用引脚极少；对外扩展主要靠底部的 HY2.0-4P 口。

## 引出引脚

| 接口 | 网络 | 功能 | 备注 |
|---|---|---|---|
| HY2.0-4P · SCL | G13 | I2C 时钟 | 底部 4P 接口（Grove-like，I2C） |
| HY2.0-4P · SDA | G4 | I2C 数据 | 也可作**外部唤醒源**（ext0 deep-sleep，见「唤醒机制」） |
| HY2.0-4P · 5V | 5V | 电源 | — |
| HY2.0-4P · GND | GND | 地 | — |

> 底部 HY2.0-4P 是本板**唯一**对外扩展口(官方文档未提及其他扩展焊盘)。

## 引脚物理排布

```text
正面视角：镜头面朝自己，镜头在上、USB-C/HY2.0-4P 在下

   ┌─────────────────────────────────┐
   │       ◉ OV3660 120° 鱼眼        │
   │                                 │
   │  RESET                    LED   │
   │                                 │
   │  PWR/WAKE                       │
   │          USB-C   HY2.0-4P       │
   └─────────────────────────────────┘

背面视角：背面标签正读，HY2.0-4P 插座在下

                  Ext.Port 丝印（左→右）
             ┌──────┬──────┬──────┬────────┐
             │ GND  │  5V  │G4/SDA│G13/SCL │
             └──────┴──────┴──────┴────────┘
```

> 官方背面标签同时给出颜色提示：GND=黑、5V=红、G4/SDA=白、G13/SCL=黄。ASCII 已完整转录，图片仅用于核对朝向。

## 原理图

- [官方 TimerCamera-F 原理图（M5 官网页）](https://docs.m5stack.com/en/unit/timercam_f)（Schematics PDF 1/2）
- [官方背面 Ext.Port 图](../../raw/assets/m5stack-timer-cameraf-port-layout.jpg)
- BM8563 接线已据原理图核实；TODO：把原理图 PDF 归档到 `raw/schematics/`。

**BM8563 引脚（原理图）：** pin1 OSCI→GND · pin2 OSCO→GND · **pin3 ~INT→`RTC_ALM`（电源开关电路）** · pin4 VSS→GND · pin5 SDA→GPIO12 · pin6 SCL→GPIO14 · pin7 CLKOUT→NC · pin8 VDD→VBAT-IN

## 示例代码

- [`examples/camera_stream/`](examples/camera_stream/) — 用 esp32-camera 按**本板 DVP 接线**初始化 OV3660 并通过串口/示例抓拍。摄像头寄存器/SCCB 细节见 [OV3660 模组页](../../modules/ov3660-camera/README.md)。
- [`examples/blink_led/`](examples/blink_led/) — 闪烁板载状态 LED（G2），最小可用烧录验证。

## 特殊说明

- **电源/开机**：PWR 键长按 2s 开机；接外部电源时设备保持开机。开机后程序须置 **POWER_HOLD(G33)=1** 维持供电（库 `Power.begin()` 自动做）；关机：软件置 HOLD=0、或按板载 RESET、或 `Power.powerOff()`。
- **两种唤醒机制**（关键）：
  1. **定时唤醒**（`Power.timerSleep(s)` / 官方 `wakeup.ino`）：设 BM8563 闹钟 → 拉低 HOLD(G33) 切主电 → 整机近乎断电（**~2μA**）。到点 BM8563 报警，**IRQ(~INT) 经 `RTC_ALM` 网络直接触发电源开关重新上电** → ESP32 冷启动。**注意：IRQ 不接任何 ESP32 GPIO**，这里不是 GPIO 唤醒。
  2. **外部唤醒**（官方 `ext_wakeup.ino`）：保持 HOLD（`gpio_hold_en`）进 deep sleep，用 **ext0 唤醒脚 = GPIO4**（即底部 HY2.0-4P 的 SDA 线复用，外部拉高唤醒）。电流高于方案 1。
- **欠压保护**：电压不足时摄像头可能触发欠压复位。可在 Arduino 初始化时禁用 brownout（`WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0)`）。
- **USB 驱动**：部分系统需手装 FTDI VCP 驱动；未识别设备名常为 "M5Stack" / "USB Serial"。
- **组合体原则**：OV3660 细节不在本页展开，统一引用 [OV3660 模组页](../../modules/ov3660-camera/README.md)。

## 相关页面

- 🔗 板载摄像头：[OV3660 Camera](../../modules/ov3660-camera/README.md)（器件细节、信号定义、驱动）
- 同芯片(ESP32) 的板子：[M5StickC Plus2](../m5stack-stickc-plus2/README.md)

## 参考来源

- `raw/datasheets/m5stack-timercam-f_pinmap.txt`（官方 TimerCam-F 页面文本快照，2025-07-21）
- `raw/datasheets/m5stack-timercam-f_library-power.txt`（官方库 Power_Class / 唤醒示例摘录，2025-07-21；据此核实 HOLD=G33 / LED=G2 / BAT_ADC=G38 / 唤醒机制）
- `raw/assets/m5stack-timer-cameraf-port-layout.jpg`（官方背面接口图，2026-07-27）
- BM8563 引脚接线：据官方原理图（用户提供，2025-07-21）
- 官方文档：<https://docs.m5stack.com/en/unit/timercam_f> · 库：<https://github.com/m5stack/TimerCam-arduino>
- TODO：把官方原理图 PDF、OV3660 datasheet 归档到 `raw/`
