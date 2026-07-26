---
type: board
name: Waveshare ESP32-S3-Matrix
slug: waveshare-esp32-s3-matrix
vendor: Waveshare
aliases: [ESP32-S3-Matrix, SKU 27119]
chip: ESP32-S3
references_modules: []
onboard_modules:
  - 8×8 WS2812 RGB LED matrix (64 LEDs, DIN=G14)
  - QMI8658 6-axis IMU (accel/gyro, I2C: SDA=GPIO11/SCL=GPIO12, INT1=GPIO10/INT2=GPIO13, addr 0x6A @ SA0=GND)
  - ME6217C33M5G 3.3V LDO (800mA)
date_added: 2025-07-21
sources:
  - raw/datasheets/waveshare-esp32-s3-matrix.txt
  - raw/datasheets/waveshare-esp32-s3-matrix-onboard-resources.txt
  - raw/datasheets/waveshare-esp32-s3-matrix-pinout-user.txt
  - raw/schematics/waveshare-esp32-s3-matrix.pdf
  - raw/matrix-s3-onboarding-issues-2026-07-21.md
tags: [board, esp32-s3, waveshare, led-matrix, imu]
---

# Waveshare ESP32-S3-Matrix

> Waveshare 紧凑型 ESP32-S3 开发板（SKU 27119），板载 **8×8 = 64 颗 WS2812 全彩 LED 矩阵** + **QMI8658 六轴 IMU**，引出 **17 个 GPIO**，USB-C 供电/烧录。适合做像素动画、小游戏、姿态交互显示。

## 概览

- 厂商 / 型号：Waveshare / ESP32-S3-Matrix（SKU 27119）
- 资料：<https://docs.waveshare.com/ESP32-S3-Matrix>

## 主控芯片

- **ESP32-S3**（Xtensa 32-bit LX7 双核，最高 240MHz）+ 超低功耗协处理器
- SRAM 512KB · ROM 384KB · RTC SRAM 16KB
- Flash **4MB**（无 PSRAM）
- Wi-Fi 2.4GHz + BLE

## 板载模组与占用引脚

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 8×8 RGB 矩阵 (WS2812 × 64) | **G14 (DIN)** | 单线 RGB | Arduino core `PIN_NEOPIXEL=14`；**字节序 = RGB（非 GRB，Adafruit NeoPixel 用 `NEO_RGB`）**；**像素映射 = 行主序、非蛇形、`idx0`=左上角**（行↓ 列→）；矩阵 **DOUT** 引出至 G37 旁焊盘（级联扩展用） |
| QMI8658 六轴 IMU | I2C: **SDA=GPIO11 · SCL=GPIO12**；INT1=GPIO10 · INT2=GPIO13 | 加速度+陀螺仪 | 7 位地址 **`0x6A`**（SA0=pin1 接 **GND**，据原理图核实）；均为专用内部 GPIO，**未引出** |
| ME6217C33M5G (LDO) | — | 3.3V 稳压 | 5V → 3.3V，Max **800mA**；`3V3(OUT)` 即其输出 |
| BOOT 按键 | GPIO0 | BOOT / strapping | 按住 BOOT + 按 RESET 进入下载模式 |
| RESET 按键 | EN / CHIP_PU | 复位 | — |
| USB-C | USB D-/D+（GPIO19/GPIO20，内部连接） | USB | 供电 + 数据 / 烧录；接 ESP32-S3 **原生 USB-Serial-JTAG**（**无 USB-UART 桥**，VID:PID=`303A:1001`，枚举 "USB JTAG/serial debug unit"）；此为板上**唯一串口通路**；D-/D+ 未引出 |

> ⚠️ 矩阵亮度不宜过高：官方提示高亮度会快速升温、可能损坏板子。
> QMI8658 占用 GPIO10–13，**均不在引出的 17 个 GPIO 内**（专用内部走线），故排针上找不到这些脚。

## 引出引脚

> 共 **20 pin** = **17 个 GPIO** + 3 电源。所有 GPIO 支持 ESP32-S3 IO MUX 全套备选功能（SPI / I2C / I2S / PWM / UART / ADC）。视角：8×8 矩阵面朝自己、丝印正读、USB-C 在顶部，左右两列焊盘从顶到底：

| # | 左列丝印 | 网络 | 备注 | | 右列丝印 | 网络 | 备注 |
|---|---|---|---|---|---|---|---|
| 1 | 5V | 5V | 电源输入（USB 5V / 外部） | | 33 | GPIO33 | GPIO |
| 2 | G | GND | 地 | | 34 | GPIO34 | GPIO |
| 3 | 3V3(OUT) | 3V3 | 经 ME6217 稳压输出 | | 35 | GPIO35 | GPIO |
| 4 | GP7 | GPIO7 | GPIO | | 36 | GPIO36 | GPIO |
| 5 | 6 | GPIO6 | GPIO | | 37 | GPIO37 | GPIO；旁有 **DOUT** 焊盘 |
| 6 | 5 | GPIO5 | GPIO | | 38 | GPIO38 | GPIO |
| 7 | 4 | GPIO4 | GPIO | | 39 | GPIO39 | GPIO |
| 8 | 3 | GPIO3 | ⚠️ strapping 引脚 | | 40 | GPIO40 | GPIO |
| 9 | 2 | GPIO2 | GPIO | | TX | GPIO43 | UART0 TX (U0TXD) |
| 10 | 1 | GPIO1 | GPIO | | RX | GPIO44 | UART0 RX (U0RXD) |

> 额外焊盘：GPIO37 旁的 **DOUT** = WS2812 矩阵数据输出（级联下一块灯板/灯条）。

## 引脚物理排布

```
   视角：8×8 LED 矩阵面朝自己 · 丝印正读 · USB-C 在顶部 · 左右两列各 10 pin

              ┌────── USB-C ──────┐
          ┌───┴───────────────────┴───┐
   顶 →  │ ● 5V               33 ●     │
         │ ● G                34 ●     │
         │ ● 3V3(OUT)         35 ●     │
         │ ● GP7              36 ●     │
         │ ● GPIO6            37 ●  ←  DOUT 焊盘(矩阵级联输出) 在此旁
         │ ● GPIO5            38 ●     │
         │ ● GPIO4            39 ●     │
         │ ● GPIO3*           40 ●     │
         │ ● GPIO2         TX(43) ●    │
   底 →  │ ● GPIO1         RX(44) ●    │
         │      [BOOT] [RESET]         │
          └────────────────────────────┘
   左列(顶→底): 5V, G, 3V3(OUT), GP7, 6, 5, 4, 3, 2, 1
   右列(顶→底): 33, 34, 35, 36, 37, 38, 39, 40, TX(43), RX(44)
   * GPIO3 = ESP32-S3 strapping 引脚（上电电平影响启动，外部用注意）
```

```text
附加焊盘：DOUT 位于右侧排针 GPIO37 焊盘内侧，用于把板载第 64 颗
WS2812 的数据输出级联到下一块灯板；它不是 GPIO37 网络。
```

> 板上两侧排针没有连续 Pin 1–20 丝印，因此按上图“LED 面、USB-C 朝上”的位置顺序识别，不自行编号。

## 原理图

- [原理图 PDF（已入库）](../../raw/schematics/waveshare-esp32-s3-matrix.pdf) — 单页：ESP32-S3 / QMI8658(U67) / 64× WS2812 链 / ME6217 LDO / USB-C。本页 QMI8658 引脚即据此核实。
- 官方 wiki（含 Pinout 图）：<https://docs.waveshare.com/ESP32-S3-Matrix>

## 示例代码

- [`examples/matrix_smile/`](examples/matrix_smile/) — 用 **GPIO14** 驱动 8×8 矩阵（Adafruit NeoPixel）：游走亮点 + 红绿蓝整屏循环。
- [`examples/imu_qmi8658/`](examples/imu_qmi8658/) — 经 I2C（**SDA=GPIO11 / SCL=GPIO12**，地址 `0x6A`）读 QMI8658 加速度+陀螺仪并打印。

> 两个示例的 `platformio.ini` 都使用通用 `esp32-s3-devkitc-1` board id，仅供展示 API。该定义按 8MB Flash 生成镜像，本板实际为 4MB；上板前必须按「特殊说明」坑 1/2 换成自定义 4MB board JSON，并使用 `esptool --no-stub` 烧录。

## 特殊说明

### 板级基础
- **GPIO3** 是 ESP32-S3 strapping 引脚，做外部 IO 时注意上电电平。
- 下载模式：按住 **BOOT** + 按 **RESET**（先松 RESET）；普通复位按 RESET。
- 矩阵 DIN=G14（`PIN_NEOPIXEL`），未引出；矩阵 DOUT 引出在 GPIO37 旁（级联扩展）。
- USB-C 接 ESP32-S3 **原生 USB-Serial-JTAG**（无 USB-UART 桥），一般免驱；枚举名 "USB JTAG/serial debug unit"、VID:PID=`303A:1001`，是板上**唯一串口通路**。

### bring-up 踩坑（详见 [`raw/matrix-s3-onboarding-issues-2026-07-21.md`](../../raw/matrix-s3-onboarding-issues-2026-07-21.md)）

从零把这块板跑起来（PlatformIO + Arduino，写 WiFi 配网小程序）踩到 7 个坑，每条「现象→根因→解法」的完整版见上方原始流水。要点速查：

| # | 现象 | 根因 | 解法 |
|---|---|---|---|
| 0 | `pio device list` 看到的串口 VID 不是 `303A`（如 `2D79`） | **USB 数据线不对**（充电线枚举成别的设备） | 换能传数据的 USB-C 线 |
| 1 | 烧录后**反复重启**（boot loop，`spi_flash: ... smaller than the size in the binary image header(8192k)` 断言） | stock `esp32-s3-devkitc-1` 其实是 -N8（8MB），elf2image 按 `upload.flash_size=8MB` 把镜像头写成 8MB，本板只有 4MB → 断言重启。**`board_build.flash_size=4MB` 没用**（elf2image 读的是 board JSON 的 `upload.flash_size`，不是 `build.flash_size`） | 写**自定义 board JSON**（4MB + `default.csv`）；完整 JSON 见原始流水 |
| 2 | `pio run -t upload` 烧不进（`No serial data received`） | esptool 软 stub 在 USB-Serial-JTAG 上切波特率/读 flash ID 时把链路搞挂 | ROM 模式 **`esptool --no-stub`** 烧录（偏移：bootloader@0x0 / partitions@0x8000 / boot_app0@0xe000 / firmware@0x10000） |
| 3 | 串口读不到 / 一读就掉线（`Device not configured` / ENXIO） | pyserial `Serial()` 的 `tcsetattr()` 拉 DTR/RTS 触发 USB-Serial-JTAG 自动复位；且默认 `Serial=UART0` 在 app 接管后 ROM 桥停止 | 读串口用 **raw `os.open()`**（不走 termios）；app 开 **`-DARDUINO_USB_CDC_ON_BOOT=1`** 让 `Serial=HWCDC` |
| 4 | 红↔绿反（红显成绿、青↔品红互换，蓝/黄/白不变） | WS2812 字节序是 **RGB**，Adafruit NeoPixel 默认 `NEO_GRB` | 声明用 **`NEO_RGB`**（字节序见上方板载模组表） |
| 5 | 扩散动画奇数行错位 | 误以为蛇形走线；实为**行主序、非蛇形** | `idxToXY: y=i/8, x=i%8`（映射见上方板载模组表） |
| 6 | 即便低占空比仍嫌亮 | WS2812 无独立亮度字节，靠 `setBrightness(b)` 降每通道占空比 | 全局设一次低亮度；`setBrightness(1)` 是硬件下限，再暗只能 app 时间抖动（代价是频闪）。亮度取舍见原始流水 |

> 标定技巧：字节序/布局用**纯 RGB 原色行**标定（低亮度下 HSV 渐变会误导、分不清）；**先修字节序、再标布局**——红绿反会让基于颜色的标定结果互相矛盾，修对后才自洽。
> ⚠️ 矩阵亮度不宜过高：官方提示高亮度会快速升温、可能损坏板子。

## 相关页面

- 同芯片(ESP32-S3) 的板子：_(暂无)_
- 暂无引用模组（板载 QMI8658 是贴片器件，按约定记在表 A，不单列模组页）

## 参考来源

- `raw/schematics/waveshare-esp32-s3-matrix.pdf`（官方原理图，2025-07-21 入库；QMI8658 / 矩阵 / 电源均据此核实）
- `raw/datasheets/waveshare-esp32-s3-matrix-pinout-user.txt`（用户据官方 Pinout 图转写，2025-07-21）
- `raw/datasheets/waveshare-esp32-s3-matrix-onboard-resources.txt`（官方 Onboard Resources，2025-07-21）
- `raw/datasheets/waveshare-esp32-s3-matrix.txt`（官方 wiki 页面文本快照，2025-07-21）
- `raw/matrix-s3-onboarding-issues-2026-07-21.md`（bring-up 踩坑流水，2026-07-21；USB-Serial-JTAG 烧录/串口、WS2812 字节序 RGB、像素映射、4MB boot loop、亮度等实战事实源）
- 官方文档：<https://docs.waveshare.com/ESP32-S3-Matrix> · 中文：<https://www.waveshare.net/wiki/ESP32-S3-Matrix>
