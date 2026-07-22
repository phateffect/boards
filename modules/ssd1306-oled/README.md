---
type: module
name: SSD1306 OLED Display
slug: ssd1306-oled
vendor: generic
device: SSD1306
interface: [I2C]
date_added: 2026-07-23
used_by: [esp32-c3-oled]
sources:
  - raw/c3-oled-screen.pdf
tags: [module, oled, ssd1306, i2c, display]
---

# SSD1306 OLED 显示模组

> **0.42 寸 SSD1306 72×40 白色单色** OLED 显示屏（1-bit 像素，非彩色），I2C 接口，板载于 ESP32-C3-OLED 开发板。屏体型号 Newvision **N042-7240TSWEG01-H16**。

> ⚠️ **重要订正（2026-07-23）**：本页原先误记为「0.96" 128×64」（录入时的默认假设，无来源）。据屏体规格书 [`raw/c3-oled-screen.pdf`](../../raw/c3-oled-screen.pdf) 核实，实际为 **0.42" 72×40**——SSD1306 IC 虽可寻址到 128×64，但本面板物理像素为 72×40（init 序列 mux=`0x27`=1/40 duty）。

## 概览

- 屏体型号 / 厂商：**Newvision N042-7240TSWEG01-H16**（据规格书）；模组按通用 SSD1306 记。
- 0.42"，白色**单色** OLED（1-bit 像素，亮/灭，非彩色）
- 板载用法：I2C 4 线连接（VCC / GND / SCL / SDA）

## 核心器件

- **SSD1306** 单芯片 OLED 驱动/控制 IC（IC 可寻址至 128×64；本面板用其 72×40 子窗）
- **分辨率：72×40** 像素；驱动占空比 **1/40 Duty**
- 像素间距 0.128 × 0.13 mm；像素尺寸 0.108 × 0.11 mm；有效区 9.196 × 5.18 mm
- 颜色：白色单色；亮度 360 cd/m²（VCC 外供时典型值）
- 接口：I2C，7 位地址 **`0x3C`**（写 `0x78`，SA0=0；**已据规格书确认**，原「待核实」消除）
- 电源：逻辑 VDD 1.65–3.3 V（板载接 3.3V）；显示屏 VCC ~7.25 V（由 SSD1306 内部 charge pump 升压，VBAT 供电）
- 工作温度：-40 ~ 85 °C

## 引脚定义

### 板级接口（c3-oled 上的 4 线 I2C 接法）

| 模组引脚 | 功能 | 连到核心板的什么 |
|---|---|---|
| VCC | 电源（逻辑 + VBAT） | 3.3V |
| GND | 地 | GND |
| SCL | I2C 时钟 | GPIO6 |
| SDA | I2C 数据 | GPIO5 |

### 屏体 COG 全引脚（规格书 N042-7240TSWEG01-H16，16 pin）

| Pin | Symbol | 功能 |
|---|---|---|
| 1, 2 | C2P / C2N | 升压电容 C2 正/负端 |
| 3, 4 | C1P / C1N | 升压电容 C1 正/负端 |
| 5 | VBAT | DC/DC 电源（charge pump 输入） |
| 6 | VSS | 逻辑地 |
| 7 | VDD | 逻辑电源 1.65–3.3V |
| 8 | BS1 | 接口选择（I2C / SPI） |
| 9 | CS | 片选（I2C 模式下拉低） |
| 10 | RES# | 复位（低有效） |
| 11 | DC | I2C 模式下 = **SA0**（地址低位） |
| 12–14 | D0, D1, D2 | I2C：D0=SCL，D1+D2 并联=SDA |
| 15 | VCOMH | COM 高电平（接电容到地） |
| 16 | VCC | 屏供电 ~7.25V（内部 charge pump 输出） |

> c3-oled 板把屏配成 **I2C 模式**（D0=SCL→GPIO6、D1+D2=SDA→GPIO5、CS 拉低、内部 charge pump 使能 VCC），对外只露出 4 线。

## 接口

- I2C，7 位地址 **`0x3C`**（写 `0x78`；SA0=DC 脚：接低 → `0x3C`，接高 → `0x3D`）。规格书 init 序列用 `write_w(0x78)` 确认。
- 初始化：`Wire.begin(SDA=5, SCL=6)`；init 序列见规格书 §4.4（含 mux=`0x27`=1/40 duty、charge pump 使能等）。也可直接用 Adafruit SSD1306 / U8g2 库（库会发标准 init）。

## 引脚物理排布

板载 COG 模组，FPC/焊接连接，非独立可拆卸；对外仅 4 线（VCC/GND/SCL/SDA）。

## 示例代码

- [ESP32-C3-OLED `examples/oled_hello_world`](../../boards/esp32-c3-oled/examples/oled_hello_world/) — Adafruit SSD1306 驱动（**72×40**，地址 `0x3C`，SDA=5 / SCL=6）：ESP32-C3 + 计数器 + 堆内存监控。

## 被哪些开发板使用

- [ESP32-C3-OLED](../../boards/esp32-c3-oled/README.md)

## 相关页面 / 参考来源

- [ESP32-C3-OLED 开发板](../../boards/esp32-c3-oled/README.md)
- `raw/c3-oled-screen.pdf` — 屏体规格书（Newvision N042-7240TSWEG01-H16，2019-05-15 版；2026-07-23 入库；72×40 / SSD1306 / I2C 0x3C / COG 16-pin 引脚 / 电气特性的事实源）
- SSD1306 主控 IC datasheet（Solomon Systek）：TODO 待入库（本规格书引用其 Technical Manual）
