---
type: module
name: M5Stack Stack-chan Body
slug: m5stack-stackchan-body
vendor: M5Stack
aliases: [StackChan robot body, Stack-chan 本体, K151 本体]
device: SCS0009 ×2 + ST25R3916 + Si12T + PY32L020 + INA226 + IRM56384 + WS2812C ×12
interface: [I2C, UART, GPIO]
date_added: 2026-08-20
used_by:
  - m5stack-stackchan
sources:
  - raw/datasheets/m5stack-stackchan-doc.txt
  - raw/schematics/m5stack-stackchan-sch-adapter.pdf
  - raw/schematics/m5stack-stackchan-sch-power.pdf
  - raw/schematics/m5stack-stackchan-sch-ring.pdf
  - raw/schematics/m5stack-stackchan-sch-touch.pdf
  - raw/datasheets/m5stack-stackchan-scs0009-servo.pdf
  - raw/datasheets/m5stack-stackchan-st25r3916.pdf
  - raw/datasheets/m5stack-stackchan-si12t.pdf
  - raw/datasheets/m5stack-stackchan-py32l020.pdf
  - raw/datasheets/m5stack-stackchan-irm56384.pdf
  - raw/datasheets/ina226.pdf
tags: [module, m5stack, stackchan, robot, servo, nfc, touch, ir, i2c, ws2812]
---

# M5Stack Stack-chan Body（机器人本体）

> Stack-chan（K151/K151-R）的**机器人本体部分**：不能独立运行，靠 CoreS3 主控插入/骑在本体上工作。含双反馈舵机（头左右 360° + 俯仰 90°）、12×WS2812C、红外收发、全功能 NFC、顶部三区触摸、INA226 电量计、PY32L020 IO 扩展、550mAh 电池与 USB-C。器件细节的事实源在本页；整机视角见 [Stack-chan 板子页](../../boards/m5stack-stackchan/README.md)。

> 本体由 4 块小板组成，官方各出一份原理图：**适配板 Adapter**（CoreS3 底座转接）、**电源板 Power**、**圈板 Ring**、**触摸板 Touch**（见「引脚物理排布」）。

## 概览

- 厂商 / 型号：M5Stack / StackChan Body（随 K151 / K151-R 整套出售；主控可单独换购 StackChan Core）
- 核心器件：SCS0009 反馈舵机 ×2、ST25R3916 NFC、Si12T 触摸（TSM12 兼容）、PY32L020 IO 扩展、INA226 电量监测、IRM56384 红外接收 + 红外发射管、WS2812C ×12
- 供电：550mAh 电池 + 底座 USB-C（数据 + 供电）
- 与主控的接口：6 根 ESP32-S3 信号线（2 舵机串口 + 2 红外 + 2 I2C，见下）+ M-Bus 机械/电气连接

## 核心器件

| 器件 | 角色 | 关键参数 | 数据手册（已归档） |
|---|---|---|---|
| SCS0009 ×2 | 反馈舵机 | X 轴 360° 连续旋转；Y 轴 90°（**限 5~85°**）；串口总线，带位置反馈 | [SCS0009.pdf](../../raw/datasheets/m5stack-stackchan-scs0009-servo.pdf) |
| WS2812C ×12 | RGB 灯（两排） | 单线协议；屏幕左上那枚兼作语音状态灯（绿=聆听/蓝=说话/灭=空闲） | — |
| 红外发射管 + IRM56384 | IR 收发 | 发射 GPIO 直驱；接收 IRM56384 解调输出 | [IRM56384.pdf](../../raw/datasheets/m5stack-stackchan-irm56384.pdf) |
| ST25R3916-AQWT | 全功能 NFC | 读卡/卡模拟/点对点；I2C 接口 | [ST25R3916_EN.pdf](../../raw/datasheets/m5stack-stackchan-st25r3916.pdf) |
| Si12T | 顶部三区触摸 | TSM12 兼容，12 通道电容触摸，本设计用 3 区 | [Si12T_EN.pdf](../../raw/datasheets/m5stack-stackchan-si12t.pdf) |
| INA226AIDGSR | 电池电量监测 | 电流/电压/功率监测，I2C | [INA226.pdf](../../raw/datasheets/ina226.pdf) |
| PY32L020 | IO 扩展 MCU | 替舵机电源与 RGB 数据扩位（见「引脚定义」）；I2C 0x6F/0x71 | [PY32L020_EN.pdf](../../raw/datasheets/m5stack-stackchan-py32l020.pdf) |

## 引脚定义

> 本体占用的 6 根 ESP32-S3 信号线（官方 PinMap 原表转录）。**舵机与 RGB 不直接占 ESP32 的 UART/GPIO 全部资源——RGB 数据由 PY32L020 的 IO14 驱动（据 PinMap 推断），ESP32 侧只出 G6/G7 舵机串口。**

| 本体信号 | 连到 ESP32-S3 | 功能 | 说明 |
|---|---|---|---|
| Servo_TX | **G6** | 舵机串口 TX | SCS0009 总线舵机（半双工协议，见其数据手册；波特率 TODO: 待核实） |
| Servo_RX | **G7** | 舵机串口 RX | 回读舵机位置反馈 |
| IR_SEND | **G5** | 红外发射 | GPIO 直驱发射管 |
| IR_REC | **G10** | 红外接收 | IRM56384 解调输出（CoreS3 M-Bus 标注 G10 为 ADC） |
| I2C_SCL | **G11** | I2C 时钟 | **与 CoreS3 板内器件共用内部总线**（M-Bus pin18） |
| I2C_SDA | **G12** | I2C 数据 | 同上（M-Bus pin17） |

**PY32L020 IO 扩展输出（经 I2C 控制）：**

| PY32L020 引脚 | 功能 | 说明 |
|---|---|---|
| IO1 | VM_EN | 电源使能（使能对象 TODO: 待核实——推断为舵机电源轨） |
| IO14 | RGB | 驱动 12×WS2812C 数据线（推断，据官方 PinMap） |

## 接口

**I2C（G11/G12，与 CoreS3 板内器件共线，地址无冲突）：**

| 器件 | 地址 | 归属 |
|---|---|---|
| 电池电量计 INA226AIDGSR | **0x41** | 本体 |
| NFC ST25R3916-AQWT | **0x50** | 本体 |
| 触摸 Si12T（TSM12 兼容） | **0x68** | 本体 |
| IO 扩展 PY32L020 | **0x6F**（ADD_SEL 低，默认）/ **0x71**（ADD_SEL 高） | 本体 |
| AXP2101 / BM8563 / AW9523 | 0x34 / 0x51 / 0x58 | CoreS3 板内 |
| FT6336U / ES7210 / AW88298 | 0x38 / 0x40 / 0x36 | CoreS3 板内 |
| GC0308 / LTR-553 / BMI270 / BMM150 | 0x21 / 0x23 / 0x69 / 0x10 | CoreS3 板内 |

> CoreS3 板内地址来自官方 CoreS3 页 I2C 地址表（快照 `raw/datasheets/m5stack-cores3-doc.txt`），已与本体的 0x41/0x50/0x68/0x6F/0x71 核对无冲突。

- **舵机串口**：G6/G7，SCS0009 协议（数据手册已归档；帧格式/波特率 TODO: 待整理）。
- **NFC**：ST25R3916 走 I2C（非 SPI），驱动见官方 [StackChan-BSP](https://github.com/m5stack/StackChan-BSP)。
- **红外**：发射 G5 普通 GPIO；接收 G10 读 IRM56384 输出。

## 引脚物理排布

> 本体无对外排针；对外只见**整机级接口**（底座 USB-C、microSD/RST、Grove 口），其排布见 [Stack-chan 板子页](../../boards/m5stack-stackchan/README.md#引脚物理排布)。本体内部 4 板的相对位置与板间线序**TODO: 待核实**（需拆机/读原理图核对）。

```text
本体 4 块小板（自上而下，据官方文档与原理图命名）

   ┌──────────────────────────┐
   │ 触摸板 Touch（头顶三区触摸） │ Si12T @ 0x68
   ├──────────────────────────┤
   │   [CoreS3 插入位/舵机关节]  │ SCS0009 ×2（X 360° / Y 90°）
   ├──────────────────────────┤
   │ 适配板 Adapter（CoreS3 底座）│ 6 根信号线 G5/G6/G7/G10/G11/G12
   ├──────────────────────────┤
   │ 电源板 Power（USB-C·550mAh）│ INA226 @ 0x41 · PY32L020 @ 0x6F
   │ 圈板 Ring                 │ WS2812C ×12 · NFC · IR
   └──────────────────────────┘
   （各板具体叠层顺序与器件归属 TODO: 待核实，勿按上图当作装配图）
```

## 示例代码

- 本体 I2C 总线扫描（对 0x41/0x50/0x68/0x6F 在位核对）：见 [Stack-chan 板子页](../../boards/m5stack-stackchan/README.md#示例代码) 的 [`examples/body_i2c_scan/`](../../boards/m5stack-stackchan/examples/body_i2c_scan/)。
- 官方 BSP（Arduino，舵机/NFC/触摸/RGB 全套驱动）：<https://github.com/m5stack/StackChan-BSP>

## 被哪些开发板使用

| 板子 | 角色 | 链接 |
|---|---|---|
| M5Stack Stack-chan（K151/K151-R） | 机器人本体（CoreS3 的载体） | [板子页](../../boards/m5stack-stackchan/README.md) |

## 相关页面 / 参考来源

- [Stack-chan 板子页](../../boards/m5stack-stackchan/README.md)（整机、CoreS3、Grove 口、下载模式）
- 官方文档：<https://docs.m5stack.com/en/StackChan>（PinMap/I2C 地址表）
- `raw/schematics/m5stack-stackchan-sch-{adapter,power,ring,touch}.pdf` — 本体 4 板官方原理图
- 固件仓库：<https://github.com/m5stack/StackChan> · BSP：<https://github.com/m5stack/StackChan-BSP>
- TODO：SCS0009 协议要点、PY32L020 寄存器用法（VM_EN/IO14）、4 板叠层与线序——原理图与数据手册均已归档，待译读补全
