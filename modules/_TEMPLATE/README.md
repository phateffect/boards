---
type: module
name: <模组名>               # 如 OV3660 Camera Module
slug: <vendor-slug>          # 如 m5stack-ov3660-cam，= 文件夹名
vendor: <厂商 或 generic>
aliases: []
device: <核心器件>           # OV3660 / BMP280 / CH224K ...
interface: []                # I2C / SPI / UART / DVP-parallel / 1-Wire / ...
date_added: 2025-07-21
used_by: []                  # lint 自动维护：引用本模组的 board slug 列表，勿手填
sources: []
tags: [module, <器件>, <特征>]
---

# <模组名>

> 一句话：什么器件、什么接口、给核心板提供什么能力。

## 概览

- 厂商 / 型号 / 别名 / 核心器件
- 照片（可选）：`![模组照片](../../raw/assets/<slug>.jpg)`

## 核心器件

- 器件型号 + 关键参数（分辨率 / 量程 / 工作电压 / 功耗 …）

## 引脚定义

> 模组引脚 → 功能 → 典型接到核心板的什么。这里是该器件信息的事实源，开发板只引用本页。

| 模组引脚 | 功能 | 典型接法 | 说明 |
|---|---|---|---|
| VCC | 电源 3.3V | 3V3 | — |
| GND | 地 | GND | — |
| SDA | I2C 数据 | 任一 GPIO | — |
| SCL | I2C 时钟 | 任一 GPIO | — |

## 接口

- 通信方式 + 关键参数（I2C 7 位地址 / SPI 片选 / UART 波特率 / DVP 数据位宽与同步信号 …）
- 初始化 / 配置要点（若有）

## 引脚物理排布

> 连接器/FPC/排针的物理排布，标清 Pin1 与视角。

```
┌────────┐
│1 VCC   │
│2 GND   │
│3 SDA   │
│4 SCL   │
└────────┘
```

## 示例代码

- [`examples/<name>/`](examples/) — 一句话说明（驱动 / 点亮 / 读数据）。

## 被哪些开发板使用

> lint 自动维护（来自 boards 的 `references_modules`）。当前：

_(无 / 待 lint 更新)_

## 相关页面 / 参考来源

- 使用本模组的开发板：`[<板名>](../../boards/<board-slug>/README.md)`
- `raw/` 资料 + 外部链接
