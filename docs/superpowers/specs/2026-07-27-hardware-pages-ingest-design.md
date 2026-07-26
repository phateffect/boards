# 五个硬件页面录入设计

日期：2026-07-27

## 目标

向 Boards Wiki 新增三个开发板页面和两个模组页面：

- M5Stack StickS3
- M5Stack StopWatch
- ESP32-S3 SuperMini
- M5Stack RoverC-Pro
- Osptek YDP420H003-V3 4.2 英寸 BWR TFT 与被动转接板

页面遵循 `CLAUDE.md` 的 frontmatter、固定章节、链接、索引和日志规范。硬件事实必须有本地资料或可信的一手资料支撑；来源之间冲突时明确记录取舍，不复制已知错误。

## 建模边界

### 开发板

以下设备能独立上电运行，放入 `boards/`：

| Slug | 名称 | 类型依据 |
|---|---|---|
| `m5stack-sticks3` | M5Stack StickS3（K150） | 集成 ESP32-S3、显示、输入、音频和电池，可独立运行 |
| `m5stack-stopwatch` | M5Stack StopWatch（C152） | 集成 ESP32-S3、AMOLED、触摸、电池和完整人机交互硬件 |
| `esp32-s3-supermini` | ESP32-S3 SuperMini | 集成 ESP32-S3、USB、电源和引出 GPIO，可独立运行 |

### 模组

以下设备需要主控板，放入 `modules/`：

| Slug | 名称 | 类型依据 |
|---|---|---|
| `m5stack-roverc-pro` | M5Stack RoverC-Pro（K036-B） | HAT/机器人底盘，以 I²C 接受外部主控命令 |
| `osptek-ydp420h003-v3` | Osptek YDP420H003-V3 BWR TFT Module | 屏体与被动转接板均无独立应用主控 |

Osptek 页面把 YDP420H003-V3 屏体和 24-pin FPC 转 1×8 排针的被动转接板合并建模，因为两者组合后才形成便于接线的 SPI 显示模组。正文分别保留屏体 FPC 引脚和转接板排针引脚，避免混淆。

ESP32-S3 SuperMini 与 Osptek 模组暂时没有组合关系。SuperMini 的 `references_modules` 不包含 Osptek，Osptek 的 `used_by` 保持空列表。

## 来源策略

### 本地归档原则

凡是对硬件事实核验、离线阅读或示例代码溯源有必要的文件，都归档到 `raw/`，包括：

- 官方原理图、规格书、协议文档和尺寸图
- 页面中承载唯一板级信息的 pinout/接线图
- 必须引用但无法由其他本地资料替代的官方页面文本快照
- Osptek 驱动所依据的原始 MIT 源文件、README、LICENSE 和固定 commit 信息

归档只保留与本次页面直接相关的材料，不下载营销轮播图、网页装饰资源、视频或无关示例。归档后的文件视为不可变事实源；后续上游更新以新增版本文件的方式录入，不覆盖旧文件。

### M5Stack 产品

StickS3、StopWatch 和 RoverC-Pro 以 M5Stack 官方产品页、官方原理图、官方协议和官方库为主要来源。实施时将必要的一手资料保存到 `raw/`，并在 frontmatter `sources` 与“参考来源”中列出；动态官方页面同时保留外部链接。

### ESP32-S3 SuperMini

板级外形、引脚图、板载 LED、供电焊盘和具体芯片版本以用户提供的 NoLogo 页面为来源。该页面把 ESP32-S3 错写成 RISC-V、单核 160MHz，因此 CPU 架构、核心数和最高频率改用 Espressif 官方 ESP32-S3 资料：

- Xtensa LX7 双核
- 最高 240MHz

页面对应的板型按 `ESP32S3FH4R2` 记录。不会把未被板级原理图或实物资料支持的通用 SuperMini 变体混入本页。

### Osptek 显示模组

本地不可变事实源：

- `raw/opstek-4.2-bwr-tft.pdf`：Osptek YDP420H003-V3 屏体规格书
- `raw/opstek-4.2-hat.pdf`：24-pin FPC 转 1×8 排针被动转接板原理图

驱动与实测编码参考：

- `https://github.com/CaiZiYuan2019/Osptek-4.2-BWR-TFT-Driver-for-ESP32-S3`
- MIT License

实施时把该仓库中实际用于派生示例的 `.h`、`.cpp`、`.ino`、`README.md`、`LICENSE` 和 commit 标识归档到 `raw/reference-code/osptek-4.2-bwr-tft-driver/`，不复制图片和 `.git` 历史。

规格书负责型号、尺寸、电气范围、FPC 引脚和 ST7306 接口事实；第三方仓库负责初始化序列、颜色编码、刷新模式和已验证的软件实现。第三方代码中的推断会标为实测/逆向结论，不冒充厂商规格。

## 页面内容

### StickS3

页面记录：

- ESP32-S3-PICO-1-N8R8、8MB Flash、8MB Octal PSRAM
- ST7789P3 135×240 LCD
- BMI270、M5PM1、ES8311、麦克风、AW8737 与扬声器
- IR TX/RX、按键、USB、250mAh 电池
- HY2.0-4P 与 16-pin Hat2 Bus
- 所有板载占用 GPIO、引出引脚和物理排布
- 下载模式、外部 5V 输出和 HAT 兼容性注意事项

### StopWatch

页面记录：

- ESP32-S3R8、16MB Flash、8MB PSRAM
- 1.75 英寸圆形 AMOLED 与触摸
- BMI270、RX8130CE RTC
- ES8311 音频、麦克风、扬声器、振动马达
- M5PM1/M5IOE1 电源与扩展管理
- 450mAh 电池、按键、HY2.0-4P 和后部扩展总线
- 下载模式、开关机方式和板载占用 GPIO

### ESP32-S3 SuperMini

页面记录：

- `ESP32S3FH4R2`、4MB Flash、2MB PSRAM
- USB-C、BOOT/RESET、板载 RGB/蓝色 LED
- 引出的 GPIO、ADC、UART、I²C、SPI 能力
- 背面 B+/B- 供电接口与外置天线改装说明
- Arduino/PlatformIO 使用通用 ESP32-S3 Dev Module 时的 USB CDC 和下载模式注意事项
- NoLogo 页面错误与官方芯片事实的取舍说明

### RoverC-Pro

页面记录：

- STM32F030 系列控制器；若官方页面不同位置给出 C6T6/C8T6 冲突，正文明确标注并以原理图丝印或芯片资料为准
- L9110S 电机驱动、四个 N20 蜗杆电机、麦克纳姆轮
- 夹爪、两个舵机接口、可更换 16340 电池
- I²C 地址 `0x38`
- StickC/HAT 接口和两个 Grove-compatible I²C 接口
- I²C 协议命令、供电、机械与兼容性说明

### Osptek YDP420H003-V3

页面记录：

- 4.2 英寸、300×400、黑/白/红反射式 TFT
- ST7306、4-wire SPI
- 屏体尺寸、有效区、工作与存储温度
- 24-pin FPC 完整定义
- 转接板 1×8 排针：VCI、GND、SCLK、SDI、RES、D/C、CS、TE
- IOVCC 与 VCI 电压范围、R1 连接关系和去耦网络
- TE 的可选用途
- 第三方驱动的颜色 nibble 编码、30KB framebuffer、1Hz LPM 与实验性 16Hz HPM

## 示例代码设计

在 `modules/osptek-ydp420h003-v3/examples/st7306_basic/` 创建独立 PlatformIO 示例：

```text
examples/st7306_basic/
├── platformio.ini
└── src/
    ├── main.cpp
    ├── OsptekBWR42.cpp
    └── OsptekBWR42.h
```

设计原则：

- 从 MIT 仓库提炼，不复制整个第三方仓库。
- 在派生文件头部注明原仓库、许可证和改动范围。
- GPIO 通过 `platformio.ini` 的 build flags 配置，不绑定 ESP32-S3 SuperMini 或任何已收录板子。
- 默认接线采用参考实现：SCLK=12、MOSI=11、CS=10、DC=9、RST=4、TE=5；页面明确说明这只是示例映射。
- 电源只使用 3.3V；不把第三方 README 中的 LED GPIO 视为显示模组引脚。
- 默认使用 1Hz LPM，16Hz HPM 通过编译选项显式启用。
- 示例显示白底、黑框、红色块和方向标记，覆盖初始化、清屏、绘图与刷新，不保留计算量较大的花朵演示。
- 驱动类接收 SPI 引脚或 SPI 实例，避免把 SCLK/MOSI 硬编码在库内部。
- framebuffer 分配失败、非法尺寸或初始化前调用更新时给出明确串口错误，不静默失败。

第三方实现将接受一次针对缓冲区映射的审查。若 30KB 缓冲区只能表达成对扫描行而非 300×400 独立像素，页面和 API 将明确写出实际逻辑分辨率；不会把未经硬件验证的映射描述成完整 300×400 独立寻址。

## 索引与交叉引用

实施时同步更新：

- `boards/README.md`：收录数由 4 更新为 7
- `modules/README.md`：收录数由 1 更新为 3
- `index.md`：增加 3 个 board 行、2 个 module 行，以及按芯片、厂商和接口的交叉视图
- `log.md`：追加本轮 ingest 和后续 lint 结果

交叉引用：

- StickS3 与 StickC Plus2 互相列为同系列页面。
- StickS3 页面可把 RoverC-Pro 列为生态相关 HAT，但不写入 `references_modules`，因为 RoverC-Pro 官方兼容目标是 StickC/StickC Plus，尚无 StickS3 兼容依据。
- RoverC-Pro 的 `used_by` 保持空列表，直到某个已收录板子明确作为实际组合引用它。
- Osptek 与 SuperMini 暂不建立任何正向或反向关系。

## 验证

实施完成后执行：

1. 解析所有正式页面的 YAML frontmatter，检查必填字段、slug 和目录名。
2. 检查固定章节是否齐全。
3. 检查所有本地 Markdown、PDF 和示例链接。
4. 检查 `index.md` 和目录 README 覆盖全部正式页面。
5. 检查 `references_modules` 与 `used_by` 双向一致。
6. 检查所有 frontmatter `sources` 指向存在的文件。
7. 编译 Osptek PlatformIO 示例；若本机缺少 PlatformIO，则记录为环境阻塞并进行静态编译配置检查。
8. 扫描页面中的芯片型号、内存、GPIO、地址和电压，确保与来源一致。
9. 运行 `git diff --check`，保证无空白错误。

## 非目标

- 不为 StickS3、StopWatch、SuperMini 或 RoverC-Pro 新写示例固件。
- 不把第三方 Osptek 仓库作为 git submodule 或完整副本纳入仓库；只在 `raw/` 保存派生示例所需的固定版本源文件和许可证。
- 不建立 SuperMini 与 Osptek 的组合关系。
- 不宣称 RoverC-Pro 与 StickS3 兼容。
- 不修改既有 `raw/` 文件。
