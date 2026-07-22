# Index

> 内容目录。每次 ingest 后更新。最后更新：**2026-07-23**（ESP32-C3-OLED 录入 + 日期订正）

## 开发板（boards）

| Slug | 名称 | 厂商 | 主控 | 板载模组 | 组合体? | 页面 |
|---|---|---|---|---|---|---|
| m5stack-stickc-plus2 | M5StickC Plus2 | M5Stack | ESP32-PICO-V3-02 | LCD/IMU/RTC/MIC/IR+LED/蜂鸣器（无PMIC） | 否 | [→](boards/m5stack-stickc-plus2/README.md) |
| waveshare-esp32-s3-matrix | ESP32-S3-Matrix | Waveshare | ESP32-S3 | 8×8 WS2812 矩阵 | 否 | [→](boards/waveshare-esp32-s3-matrix/README.md) |
| m5stack-timer-cameraf | Timer Camera F | M5Stack | ESP32-D0WDQ6-V3 | OV3660 摄像头/RTC/LED | **是（+OV3660）** | [→](boards/m5stack-timer-cameraf/README.md) |
| esp32-c3-oled | ESP32-C3-OLED | DIY | ESP32-C3 | SSD1306 OLED / LDO / LED | **是（+SSD1306）** | [→](boards/esp32-c3-oled/README.md) |

## 模组小板（modules）

| Slug | 名称 | 核心器件 | 接口 | 被用于 | 页面 |
|---|---|---|---|---|---|
| ov3660-camera | OV3660 Camera | OV3660 | DVP-parallel / SCCB | Timer Camera F | [→](modules/ov3660-camera/README.md) |
| ssd1306-oled | SSD1306 OLED | SSD1306 | I2C | ESP32-C3-OLED | [→](modules/ssd1306-oled/README.md) |

## 交叉视图

### 按主控芯片
- **ESP32**（含 PICO / D0WDQ6 变体）：[StickC Plus2](boards/m5stack-stickc-plus2/README.md)、[Timer Camera F](boards/m5stack-timer-cameraf/README.md)
- **ESP32-S3**：[ESP32-S3-Matrix](boards/waveshare-esp32-s3-matrix/README.md)
- **ESP32-C3**：[ESP32-C3-OLED](boards/esp32-c3-oled/README.md)

### 按厂商
- **M5Stack**：[StickC Plus2](boards/m5stack-stickc-plus2/README.md)、[Timer Camera F](boards/m5stack-timer-cameraf/README.md)
- **Waveshare**：[ESP32-S3-Matrix](boards/waveshare-esp32-s3-matrix/README.md)
- **DIY**：[ESP32-C3-OLED](boards/esp32-c3-oled/README.md)

### 对比页 / 综合分析
_(查询中沉淀下来的对比、分析页面放这里。)_ TODO 候选：ESP32 摄像头板对比、可穿戴板对比。

### 组合体关系
- [Timer Camera F](boards/m5stack-timer-cameraf/README.md) ──内含──▶ [OV3660 Camera](modules/ov3660-camera/README.md)
- [ESP32-C3-OLED](boards/esp32-c3-oled/README.md) ──内含──▶ [SSD1306 OLED](modules/ssd1306-oled/README.md)

---

_导航：[README](README.md) · [CLAUDE.md（维护规范）](CLAUDE.md) · [log](log.md)_
