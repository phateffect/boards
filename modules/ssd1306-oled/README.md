---
type: module
name: SSD1306 OLED Display
slug: ssd1306-oled
vendor: generic
device: SSD1306
interface: [I2C]
date_added: 2026-07-23
used_by: [esp32-c3-oled]
sources: []
tags: [module, oled, ssd1306, i2c, display]
---

# SSD1306 OLED 显示模组

> 0.96 寸 **SSD1306 128×64** 白色 OLED 显示屏，I2C 接口，板载于 ESP32-C3-OLED 开发板。

## 概览

- 厂商 / 型号：generic / SSD1306 128×64
- 0.96 寸，白色 OLED
- I2C 4 线连接（VCC / GND / SCL / SDA）

## 核心器件

- **SSD1306** 单芯片 OLED 驱动
- 分辨率：128×64 像素
- 接口：I2C（默认地址 `0x3C`，待核实）
- 工作电压：3.3V（板载已接 3.3V）

## 引脚定义

| 模组引脚 | 功能 | 连到核心板的什么 |
|---|---|---|
| VCC | 电源 | 3.3V |
| GND | 地 | GND |
| SCL | I2C 时钟 | GPIO6 |
| SDA | I2C 数据 | GPIO5 |

## 接口

- I2C，默认 7 位地址 `0x3C`（待核实，SSD1306 常见默认地址）
- 需 `Wire.begin(SDA_PIN, SCL_PIN)` 初始化

## 引脚物理排布

板载模组，4 线 FPC 或焊接连接，非独立可拆卸。

## 示例代码

- 待补。建议使用 Adafruit SSD1306 或 U8g2 库驱动。

## 被哪些开发板使用

- [ESP32-C3-OLED](../../boards/esp32-c3-oled/README.md)

## 相关页面 / 参考来源

- [ESP32-C3-OLED 开发板](../../boards/esp32-c3-oled/README.md)
- SSD1306 datasheet：TODO 待入库
