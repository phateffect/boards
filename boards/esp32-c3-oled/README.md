---
type: board
name: ESP32-C3-OLED
slug: esp32-c3-oled
vendor: DIY
aliases: [ESP32-C3 with OLED]
chip: ESP32-C3
references_modules: []
onboard_modules:
  - Newvision N042-7240TSWEG01-H16, SSD1306 72×40 白色单色 OLED, 0.42" (I2C: SCL=GPIO6, SDA=GPIO5, addr 0x3C)
  - ME6211C33 LDO 3.3V
  - Power LED (红色)
date_added: 2026-07-23
sources:
  - raw/schematics/esp32-c3-oled.pdf
  - raw/c3-oled.ino
  - raw/c3-oled-screen.pdf
tags: [board, esp32-c3, oled, ssd1306, i2c, spi, diy]
---

# ESP32-C3-OLED

> 自制 ESP32-C3 开发板，板载 **SSD1306 72×40 白色单色 OLED 显示屏（0.42"）**，USB-C 供电/烧录；两侧共 16 个排针位，引出 13 路 GPIO 信号，适合小型物联网显示项目。

## 概览

- 厂商 / 型号：自制 / ESP32-C3-OLED
- USB-C 供电 + 烧录
- 板载 SSD1306 72×40 白色单色 OLED（0.42"，I2C 连接）
- BOOT + RESET 按键

## 主控芯片

- **ESP32-C3**（RISC-V 32-bit 单核，最高 160MHz）
- Flash **4MB**，无 PSRAM
- Wi-Fi 2.4GHz + BLE 5.0
- QFN32 5×5mm 封装

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| Newvision N042-7240TSWEG01-H16 OLED | GPIO6(SCL), GPIO5(SDA) | I2C | SSD1306 驱动，0.42"、72×40、白色单色，7 位地址 `0x3C` |
| ME6211C33 LDO | — | 3.3V 稳压 | USB 5V → 3.3V，Max 300mA（据原理图 C1=4.7μF 输入、C2=4.7μF 输出） |
| Power LED (红色) | VCC 经 R(4.7K) | 电源指示 | 常亮，不可编程 |
| BOOT 按键 | GPIO9 | BOOT / strapping | 按住 BOOT + 按 RESET 进入下载模式 |
| RESET 按键 | EN / CHIP_PU | 复位 | 拉低 EN 复位 |
| USB Type-C | GPIO18(USB D-), GPIO19(USB D+) | USB Serial/JTAG | 原生 USB 供电 + 数据/烧录，无外部 USB-UART 桥；CC1/CC2 各经 5.1K 电阻下拉到 GND |

### 板载 OLED 规格

- 屏体型号：**Newvision N042-7240TSWEG01-H16**；该型号仅描述本板实际安装的屏体，不代表所有 SSD1306 显示屏。
- 驱动 IC：**SSD1306**；本屏物理分辨率 **72×40**、0.42 英寸、白色单色、1/40 duty。SSD1306 也可用于其他尺寸和分辨率的面板。
- 板级接口：I2C，7 位地址 **`0x3C`**，SDA=GPIO5、SCL=GPIO6；初始化使用 `Wire.begin(5, 6)`。
- 板上接 3.3V，屏体所需显示电压由 SSD1306 内部 charge pump 产生。
- 规格来源：[屏体规格书](../../raw/c3-oled-screen.pdf)。

> ⚠️ 板载 OLED 占用 GPIO5/GPIO6（I2C）。外部 I2C 设备可与 OLED 共用这一路总线（地址不得冲突），也可把另一控制器映射到其他空闲 GPIO，例如 GPIO7/GPIO10。不要把 GPIO9 当作普通、无约束的 SCL 首选：它既是 strapping 引脚，又连接 BOOT 按键；外设下拉、额外上拉/下拉或按下 BOOT 都可能干扰启动或总线时钟。

## 引出引脚

> 两侧排针共 **16 个物理位置** = **3 个电源/地** + **13 路 GPIO 信号**。其中 11 路以数字/模拟标签引出（GPIO0–GPIO10），另 2 路以 UART0 的 RX/TX 标签引出（GPIO20/GPIO21）；“11 GPIO”只是在不计 RX/TX 时的标签数量，不是 GPIO 信号总数。视角：USB-C 在顶部，正读丝印：

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 5V | 5V | 电源输入 | USB 5V 直通 |
| GND | GND | 地 | — |
| 3V3 | 3V3 | 电源输出 | 经 ME6211C33 稳压，≤300mA |
| RX | GPIO20 | UART0 RX | 串口接收 |
| TX | GPIO21 | UART0 TX | 串口发送 |
| A0 | GPIO0 | ADC1_CH0 | 模拟输入 |
| A1 | GPIO1 | ADC1_CH1 | 模拟输入 |
| A2 | GPIO2 | ADC1_CH2 | 模拟输入 |
| A3 | GPIO3 | ADC1_CH3 / SPI SS | SPI 片选（默认） |
| A4 | GPIO4 | ADC1_CH4 / SPI MOSI | SPI 主出从入 |
| A5 | GPIO5 | ADC1_CH5 / SPI MISO / I2C SDA | ⚠️ 已被板载 OLED 占用（I2C SDA） |
| — | GPIO6 | SPI SS / I2C SCL | ⚠️ 已被板载 OLED 占用（I2C SCL） |
| — | GPIO7 | GPIO | 通用 IO |
| SDA | GPIO8 | GPIO / 可映射为 I2C SDA | 丝印标为 SDA；可按需映射外设信号 |
| SCL | GPIO9 | GPIO / 可映射为 I2C SCL | ⚠️ strapping + BOOT 按键；外设或按键可能将其拉低 |
| — | GPIO10 | GPIO | 通用 IO |

> GPIO0–4 支持 ADC；GPIO3–7 可用于 SPI；GPIO8/9 丝印为 SDA/SCL，但 GPIO9 是 strapping 引脚且与 BOOT 按键共网；GPIO20–21 为 UART0。ESP32-C3 的外设信号可经 GPIO matrix 映射，表中的 I2C/SPI 用途不是唯一固定功能。

## 引脚物理排布

```
    视角：正面朝自己 · 丝印正读 · USB-C 在顶部

              ┌────── USB-C ──────┐
          ┌───┴───────────────────┴───┐
          │ ● 5V                 10 ●  │
          │ ● GND                 9 ●  │  SCL (GPIO9)
          │ ● 3V3                 8 ●  │  SDA (GPIO8)
          │ ● RX (GPIO20)         7 ●  │  GPIO7
          │ ● TX (GPIO21)         6 ●  │  (GPIO6, OLED SCL)
          │ ● A0 (GPIO0)          5 ●  │  (GPIO5, OLED SDA)
          │ ● A1 (GPIO1)          4 ●  │  A4 (GPIO4, MOSI)
          │ ● A2 (GPIO2)          3 ●  │  A3 (GPIO3, SS)
          │      [BOOT] [RST]          │
          └────────────────────────────┘
    左列(顶→底): 5V, GND, 3V3, RX, TX, A0, A1, A2
    右列(顶→底): 10, 9(SCL), 8(SDA), 7, 6(OLED), 5(OLED), 4(MOSI), 3(SS)
```

## 原理图

- [原理图 PDF（已入库）](../../raw/schematics/esp32-c3-oled.pdf) — 含 USB Type-C、ME6211C33 LDO、ESP32-C3 QFN32、SSD1306 OLED 模块、BOOT/RESET 按键、Power LED。
- 引脚图（图片）：用户提供，存于对话记录。

## 示例代码

- [`examples/oled_hello_world/`](examples/oled_hello_world/) — 板载 OLED 驱动示例：显示板名、计数器与堆内存监控，使用 Adafruit SSD1306 库。

## 特殊说明

- **GPIO9** 是 ESP32-C3 strapping 引脚，上电电平影响启动模式。做外部 IO 时注意。
- **GPIO9 与 BOOT 按键共网**（板上 10K 上拉，按键按下时接地）。若把它用作 I2C SCL，外设保持低电平、附加上下拉或按下 BOOT 都会影响启动或正常通信；优先选其他空闲、非 strapping GPIO。
- 下载模式：按住 **BOOT** + 按 **RESET**（先松 RESET）。
- 板载 OLED 通过 I2C 连接（SCL=GPIO6, SDA=GPIO5），初始化时须显式使用 `Wire.begin(5, 6)`。
- USB Type-C 的 D-/D+ 分别连接 ESP32-C3 的 **GPIO18/GPIO19 原生 USB Serial/JTAG**，无外部 USB-UART 桥；GPIO20/GPIO21 是排针上的 UART0 RX/TX，与 USB 数据线不是同一组信号。
- Arduino 的 `Serial` 若要通过 USB-C 输出，需启用原生 USB CDC，例如 PlatformIO `build_flags` 设置 `ARDUINO_USB_MODE=1` 与 `ARDUINO_USB_CDC_ON_BOOT=1`；本页示例已包含该配置。
- 外部 I2C 设备可与 GPIO5/6 上的 OLED 共用总线；如需独立总线，优先映射到合适的空闲 GPIO（例如 GPIO7/GPIO10），避开 GPIO9 的 BOOT/strapping 冲突。

## 相关页面

- 同芯片(ESP32-C3) 的板子：_(暂无)_

## 参考来源

- `raw/schematics/esp32-c3-oled.pdf`（原理图 PDF，2026-07-23 入库）
- `raw/c3-oled.ino`（原始 Arduino 示例，2026-07-23 入库）
- `raw/c3-oled-screen.pdf`（Newvision N042-7240TSWEG01-H16 屏体规格书：0.42"、72×40、SSD1306、I2C `0x3C`）
