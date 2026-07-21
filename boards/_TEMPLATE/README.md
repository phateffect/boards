---
type: board
name: <厂商 型号>           # 如 M5Stack Timer CameraF
slug: <vendor-slug>          # 如 m5stack-timer-cameraf，= 文件夹名
vendor: <厂商>               # M5Stack / Espressif / LilyGO / Seeed / ...
aliases: []                  # 别名，如 [Timer Camera F, TimerCam F]
chip: <主控芯片>             # ESP32 / ESP32-S3 / ESP32-C3 / ESP32-C6 / RP2040 ...
references_modules: []       # 本板内含/使用的模组 slug（见 CLAUDE.md §8）。纯开发板留 []
onboard_modules: []          # 板载模组简表（详见表 A）
date_added: 2025-07-21
sources: []                  # raw/ 下相对路径，如 raw/datasheets/xxx.pdf
tags: [board, <芯片>, <特征>]
---

# <中文名> <英文名>

> 一句话：这块板是什么、主控是什么、主要卖点。

## 概览

- 厂商 / 型号 / 别名
- 板子照片：`![板子照片](../../raw/assets/<slug>.jpg)`（有则放）
- 购买 / 资料链接

## 主控芯片

- 型号、封装、Flash / PSRAM 容量
- 如有芯片 wiki 页或官方手册，链接过来

## 板载模组与占用引脚

> 表 A：板子内部“账”——哪些板载器件吃掉了哪些引脚。已被占用、未引出的引脚写这里，不要进“引出引脚”表。

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| USB-UART (CH340) | GPIO43(TX0), GPIO44(RX0) | UART0 | 下载/调试串口 |
| 板载 PSRAM | GPIO26–GPIO32 | SPI/QSPI | 已占用，勿复用 |
| 板载 LED (WS2812) | GPIO2 | RMT | 可编程 RGB |

<!-- 若是“核心板+模组”组合体：模组那行只写“见模组页”，不重复引脚。见 CLAUDE.md §8 -->
<!-- 例：| 摄像头模组 | (见模组页) | DVP | 使用 OV3660，详见 [OV3660 模组](../../modules/<module-slug>/README.md) | -->

## 引出引脚

> 表 B：对外暴露的引脚。丝印 = 板子印的字；网络 = 真实 GPIO/电源。

| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 0 | GPIO0 | BOOT / ADC2_CH1 | strapping |
| 3V3 | 3V3 | 电源输出 | ≤500mA |
| GND | GND | 地 | — |

## 引脚物理排布

> 标清视角与 Pin1 位置。排针/邮票孔用 ASCII 图；Grove/JST/FPC 用表。

```
           ┌──────────────┐
     3V3 ──┤1          40 ├── GND
    GPIO0──┤2          39 ├── GPIO38
           └──────────────┘
     (视角：USB 口朝自己，Pin1 在左上)
```

或命名连接器表：

| 丝印 | 类型 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|---|
| PORT.A | Grove | VCC | GND | SDA(GPIO12) | SCL(GPIO11) |

## 原理图

- [原理图 PDF](../../raw/schematics/<slug>.pdf)（标注页码/网络名）

## 示例代码

- [`examples/<name>/`](examples/) — 一句话说明每个示例干啥。
  - `blink` — 点亮板载 LED
  - `camera` — 调用摄像头（若引用了摄像头模组）

## 特殊说明

- strapping 引脚、烧录方式（USB / UART / JTAG）、上电时序、已知坑。

## 相关页面

- 引用的模组：[<模组名>](../../modules/<module-slug>/README.md)
- 同芯片的其他板子 / 对比页

## 参考来源

- `raw/` 下资料清单
- 外部链接（官方页面、社区帖）
