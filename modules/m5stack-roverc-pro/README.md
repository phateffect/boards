---
type: module
name: M5Stack RoverC-Pro
slug: m5stack-roverc-pro
vendor: M5Stack
aliases: [RoverC Pro, RoverC-Pro, SKU K036-B]
device: STM32F030 + L9110S
interface: [I2C, HAT]
date_added: 2026-07-27
used_by: []
sources:
  - raw/datasheets/m5stack-roverc-pro.pdf
  - raw/datasheets/m5stack-roverc-pro-i2c-protocol.pdf
tags: [module, m5stack, rover, mecanum, i2c, hat]
---

# M5Stack RoverC-Pro

> 面向 Stick 系列核心板的全向移动 HAT，内含 STM32F030、四个麦克纳姆轮电机、夹爪舵机和 16340 电池。

## 概览

- 厂商 / 型号：M5Stack RoverC-Pro（SKU K036-B）
- 四个 N20 蜗杆减速电机、L9110S 电机驱动、两路舵机接口与夹爪
- 16340 700mAh 可充电电池
- 官方资料：<https://docs.m5stack.com/en/hat/hat_roverc_pro>

## 核心器件

- 控制器：STM32F030。官方同一页的描述区写 `STM32F030C6T6`，规格表写 `STM32F030C8T6`，具体后缀存在冲突，故在核实 BOM 前不选定其一。
- 电机驱动：L9110S；四个 N20 蜗杆减速电机配麦克纳姆轮。
- I2C 从设备地址：`0x38`。

## 引脚定义

### M5StickC Plus2 直插 HAT

| RoverC-Pro 引脚 | 功能 | M5StickC Plus2 | 说明 |
|---|---|---|---|
| SCL | I2C 时钟 | **G26** | 官方 RoverC-Pro PinMap |
| SDA | I2C 数据 | **G0** | G0 同时与 StickC Plus2 麦克风 CLK 复用 |
| 5V | 电源 | **5V** | HAT 直插供电 |
| GND | 地 | **GND** | 必须共地 |

### Grove I2C 改线（StickS3）

这是用户提供的可用接法，不是把 RoverC-Pro Grove 口的 5V 线原样插到 StickS3 Grove：

| RoverC-Pro | StickS3 | 说明 |
|---|---|---|
| GND | GND | **必须共地** |
| 电源 | **HAT2 `3V3_L2`** | 给 RoverC-Pro 提供 3.3V |
| SDA | G9（SDA） | 数据线必须对应连接 |
| SCL | G10（SCL） | 时钟线必须对应连接 |

> ⚠️ 改线时不要同时连接 RoverC-Pro Grove 的 5V 与 `3V3_L2`，也不要把 SDA/SCL 交叉。官方 Grove 连接器标的是 5V 供电，而这里是独立引出信号并改由 `3V3_L2` 供电的实用接法。

## 接口

- I2C：7-bit 地址 `0x38`，推荐 100–400kHz。
- 电机速度：寄存器 `0x00`–`0x03` 对应四个电机，有符号值 `-127`–`127`。
- 舵机角度：`0x10`/`0x11`，范围 `0`–`180`。
- 舵机脉宽：`0x20`–`0x23`，每路高/低字节，范围 500–2500µs，50Hz。

## 引脚物理排布

| 接口 | 类型 | 信号 |
|---|---|---|
| Stick HAT | 直插 | SCL=G26、SDA=G0、5V、GND（以 StickC Plus2 为例） |
| Grove I2C ×2 | HY2.0-4P | SCL、SDA、5V、GND |
| Servo ×2 | 3-pin | PWM、电源、GND |

## 示例代码

- 无；控制时向 `0x38` 的速度/舵机寄存器写入数据。

## 被哪些开发板使用

_(无；`references_modules` 仅表示板载/固定组合关系，外接兼容性不写入该字段。)_

## 相关页面 / 参考来源

- 可直插的核心板：[M5StickC Plus2](../../boards/m5stack-stickc-plus2/README.md)
- 官方列出的兼容核心板及 Grove 改线示例：[StickS3](../../boards/m5stack-sticks3/README.md)
- [官方产品页归档](../../raw/datasheets/m5stack-roverc-pro.pdf)
- [官方 I2C 协议](../../raw/datasheets/m5stack-roverc-pro-i2c-protocol.pdf)
- 官方产品页：<https://docs.m5stack.com/en/hat/hat_roverc_pro>
