# Index

> 内容目录。每次 ingest 后更新。最后更新：**2026-08-20**（新增 M5Stack Stack-chan 及其机器人本体模组）

## 开发板（boards）

| Slug | 名称 | 厂商 | 主控 | 板载模组 | 组合体? | 页面 |
|---|---|---|---|---|---|---|
| m5stack-stickc-plus2 | M5StickC Plus2 | M5Stack | ESP32-PICO-V3-02 | LCD/IMU/RTC/MIC/IR+LED/蜂鸣器（无PMIC） | 否 | [→](boards/m5stack-stickc-plus2/README.md) |
| m5stack-sticks3 | M5Stack StickS3 | M5Stack | ESP32-S3-PICO-1-N8R8 | LCD/IMU/音频/红外/M5PM1 | 否 | [→](boards/m5stack-sticks3/README.md) |
| m5stack-stopwatch | M5Stack StopWatch | M5Stack | ESP32-S3R8 | AMOLED/触摸/IMU/RTC/音频/振动 | 否 | [→](boards/m5stack-stopwatch/README.md) |
| esp32-s3-supermini | ESP32-S3 SuperMini | generic | ESP32-S3FH4R2 | WS2812+红色 LED/锂电充电 | 否 | [→](boards/esp32-s3-supermini/README.md) |
| esp32-s3-ai-board | ESP32-S3-AI-Board | OpenJumper | ESP32-S3-WROOM-1-N16R8 | ST7789/数字麦克风/I2S 音频/WS2812/锂电充电 | 否 | [→](boards/esp32-s3-ai-board/README.md) |
| waveshare-esp32-s3-matrix | ESP32-S3-Matrix | Waveshare | ESP32-S3 | 8×8 WS2812 矩阵 | 否 | [→](boards/waveshare-esp32-s3-matrix/README.md) |
| m5stack-timer-cameraf | Timer Camera F | M5Stack | ESP32-D0WDQ6-V3 | OV3660 摄像头/RTC/LED | **是（+OV3660）** | [→](boards/m5stack-timer-cameraf/README.md) |
| esp32-c3-oled | ESP32-C3-OLED | DIY | ESP32-C3 | Newvision N042-7240TSWEG01-H16（SSD1306 0.42" 72×40）/ LDO / LED；16-pin / 13 GPIO signals | 否 | [→](boards/esp32-c3-oled/README.md) |
| m5stack-stackchan | M5Stack Stack-chan | M5Stack | ESP32-S3（CoreS3，16MB Flash/8MB PSRAM） | CoreS3（LCD/触摸/GC0308/IMU/音频/SD/Grove×3）+ 本体（舵机×2/RGB×12/IR/NFC/触摸/电量计） | **是（+本体模组）** | [→](boards/m5stack-stackchan/README.md) |

## 模组小板（modules）

| Slug | 名称 | 核心器件 | 接口 | 被用于 | 页面 |
|---|---|---|---|---|---|
| ov3660-camera | OV3660 Camera | OV3660 | DVP-parallel / SCCB | Timer Camera F | [→](modules/ov3660-camera/README.md) |
| m5stack-roverc-pro | M5Stack RoverC-Pro | STM32F030 + L9110S | I2C / HAT | 外接兼容 StickC Plus2 / StickS3 | [→](modules/m5stack-roverc-pro/README.md) |
| osptek-ydp420h003-v3 | Osptek YDP420H003-V3 | ST7306 | SPI | 通用外接 | [→](modules/osptek-ydp420h003-v3/README.md) |
| m5stack-stackchan-body | M5Stack Stack-chan Body | SCS0009×2 + ST25R3916 + Si12T + PY32L020 + INA226 + IRM56384 | I2C / UART / GPIO | Stack-chan（K151） | [→](modules/m5stack-stackchan-body/README.md) |

## 交叉视图

### 按主控芯片
- **ESP32**（含 PICO / D0WDQ6 变体）：[StickC Plus2](boards/m5stack-stickc-plus2/README.md)、[Timer Camera F](boards/m5stack-timer-cameraf/README.md)
- **ESP32-S3**：[StickS3](boards/m5stack-sticks3/README.md)、[StopWatch](boards/m5stack-stopwatch/README.md)、[ESP32-S3 SuperMini](boards/esp32-s3-supermini/README.md)、[ESP32-S3-AI-Board](boards/esp32-s3-ai-board/README.md)、[ESP32-S3-Matrix](boards/waveshare-esp32-s3-matrix/README.md)、[Stack-chan](boards/m5stack-stackchan/README.md)
- **ESP32-C3**：[ESP32-C3-OLED](boards/esp32-c3-oled/README.md)

### 按厂商
- **M5Stack**：[StickC Plus2](boards/m5stack-stickc-plus2/README.md)、[StickS3](boards/m5stack-sticks3/README.md)、[StopWatch](boards/m5stack-stopwatch/README.md)、[Timer Camera F](boards/m5stack-timer-cameraf/README.md)、[Stack-chan](boards/m5stack-stackchan/README.md)、[RoverC-Pro](modules/m5stack-roverc-pro/README.md)、[Stack-chan Body](modules/m5stack-stackchan-body/README.md)
- **Waveshare**：[ESP32-S3-Matrix](boards/waveshare-esp32-s3-matrix/README.md)
- **Osptek**：[YDP420H003-V3](modules/osptek-ydp420h003-v3/README.md)
- **OpenJumper**：[ESP32-S3-AI-Board](boards/esp32-s3-ai-board/README.md)
- **DIY**：[ESP32-C3-OLED](boards/esp32-c3-oled/README.md)
- **generic**：[ESP32-S3 SuperMini](boards/esp32-s3-supermini/README.md)

### 对比页 / 综合分析
_(查询中沉淀下来的对比、分析页面放这里。)_ TODO 候选：ESP32 摄像头板对比、可穿戴板对比。

### 组合体关系
- [Timer Camera F](boards/m5stack-timer-cameraf/README.md) ──内含──▶ [OV3660 Camera](modules/ov3660-camera/README.md)
- [Stack-chan](boards/m5stack-stackchan/README.md) ──内含──▶ [Stack-chan Body](modules/m5stack-stackchan-body/README.md)（本体器件挂 CoreS3 内部 I2C，主控为整机自带 CoreS3）

---

_导航：[README](README.md) · [CLAUDE.md（维护规范）](CLAUDE.md) · [log](log.md)_
