# Log

> 追加式流水账。**只追加，不改历史条目**。
> 每条以 `## [YYYY-MM-DD] <op> | <标题>` 开头（op ∈ init/ingest/query/lint/update/fix），便于 `grep "^## \[" log.md | tail` 查询。

## [2025-07-21] init | 初始化 Boards Wiki 框架

- 建立 README.md（模式说明）+ CLAUDE.md（维护规范/schema）
- 建立 index.md / log.md
- 建立 `boards/` `modules/` `raw/` 目录结构 + 各自 README + `_TEMPLATE/` 模板
- 约定：slug 命名、相对 markdown 链接、两张引脚表 + 物理排布图、核心板+模组组合体的处理（§8）
- 待办：录入第一块真实板子（用户后续提供资料）

## [2025-07-21] ingest | M5StickC Plus2 / Waveshare ESP32-S3-Matrix / M5Stack Timer Camera F (+ OV3660 模组)

- 抓取官方文档文本快照入 `raw/datasheets/`（3 份，作为引脚事实源）
- 新增 boards:
  - `boards/m5stack-stickc-plus2/` — ESP32-PICO-D4；引脚取自官方 Plus 文档（Plus2 共用）；含 `examples/blink_led`
  - `boards/waveshare-esp32-s3-matrix/` — ESP32-S3，8×8 WS2812 矩阵在 **G14**；20 引出引脚的逐脚定义在官方 Pinout 图(图片)，标 TODO；含 `examples/matrix_smile`
  - `boards/m5stack-timer-cameraf/` — ESP32-D0WDQ6-V3 + 8MB PSRAM，**组合体**：摄像头引用模组页，本页只给板级 DVP 接线；含 `examples/blink_led`、`examples/camera_stream`(esp32-camera+OV3660)
- 新增 modules:
  - `modules/ov3660-camera/` — OV3660 器件事实源（信号定义/接口/SCCB），`used_by: [m5stack-timer-cameraf]`
- 更新 `index.md`（3 板 + 1 模组 + 交叉视图 + 组合体关系）
- TODO（lint 候选）：
  - Waveshare ESP32-S3-Matrix 的 20 引出引脚逐脚定义（待 Pinout 图入库）
  - StickC Plus2 的 BtnA/B 与 G37/G39 对应、HAT 精确排布、电池容量、Plus2 是否换 PMU
  - TimerCam F 的 BM8563 IRQ 具体唤醒 GPIO、背面 U 形焊盘定义
  - 各板官方原理图 PDF / 引脚图 / OV3660 datasheet 入 `raw/`

## [2025-07-21] lint | 首次体检：修 2 处，剩 8 处数据缺口待源

检查项：结构/链接、反向链接(refs<->used_by)、index 覆盖、孤儿页、芯片一致性、示例与页面引脚一致性、stale 文案。

**修复（已改）：**
- stale：`boards/README.md`、`modules/README.md` 的「已收录」还写「当前为空」→ 更新为真实清单（3 板 / 1 模组）。
- 健壮性：`m5stack-timer-cameraf` frontmatter 的 `references_modules` 列表项带行内 YAML 注释，导致朴素解析器误判反向链接断裂 → 去掉行内注释，注释挪到正文（frontmatter 纯 slug，便于 Dataview/lint 解析）。

**通过：**
- 链接：37 个相对 md 链接全部解析（0 broken）；index 链接 0 broken。
- index 覆盖：4 个页面全部入 index（无遗漏、无悬空）。
- 反向链接：`TimerCamF --refs--> ov3660-camera`，`ov3660.used_by = [timer-cameraf]`，双向一致（0 problems）。
- 芯片一致性：index 与各页 frontmatter 完全一致（ESP32-PICO-D4 / ESP32-S3 / ESP32-D0WDQ6-V3）。
- 示例↔页面引脚一致：StickC LED=G10、Matrix=G14、TimerCam LED=G2、camera_stream DVP 接线与页面表一致。
- §8 不重复原则：OV3660 器件细节只在模组页；板子页仅给板级 DVP 接线，无矛盾。

**剩余数据缺口（待源，按优先级）：**
1. [waveshare-esp32-s3-matrix] 20 个引出 GPIO 逐脚定义（官方只在 Pinout 图）— 需图入 `raw/photos/`。
2. [m5stack-timer-cameraf] BM8563 RTC 唤醒 IRQ 具体 GPIO；背面 U 形焊盘定义 — 需原理图 PDF。
3. [m5stack-stickc-plus2] BtnA/B 与 G37/G39 的精确对应（M5 库定义）；红色 LED 极性。
4. [m5stack-stickc-plus2] 电池容量（约 200mAh 待核实）；Plus2 是否仍为 AXP192 PMU。
5. [ov3660-camera] SCCB 默认 7 位地址（0x3C 待核实）。
6. 所有板：官方原理图 PDF / 引脚图 / OV3660 datasheet 入 `raw/`。
7. 验证示例 platformio board id：`m5stick-c`、`esp32-s3-devkitc-1`、`m5stack-timer-cam` 是否在用平台版本中存在。
8. 候选新页：ESP32 摄像头板对比页、可穿戴板对比页（查询沉淀）。

## [2025-07-21] update | Waveshare ESP32-S3-Matrix：原理图入库 + 引脚填实（QMI8658 核实）

- 用户补充官方 wiki（中/英）+ 用户手转 Pinout；新增发现：板载 **QMI8658 六轴 IMU**（之前缺漏）。
- 用户放入官方原理图 PDF → 入库 `raw/schematics/waveshare-esp32-s3-matrix.pdf`。
- 据原理图（全局网络标号）核实 Waveshare ESP32-S3-Matrix 全部引脚：
  - **QMI8658（U67）I2C**：SDA=**GPIO11**、SCL=**GPIO12**；INT1=**GPIO10**、INT2=**GPIO13**；地址 `0x6A`。GPIO10–13 均为内部专用，**不在引出的 17 个 GPIO 内**。
  - **矩阵**：DIN(LED_DIN)=**GPIO14**；级联输出 DOUT 为测试点 TP1（板子上靠近 GPIO37 的 “Dout” 口）。
  - **USB**：USB0_N/P = GPIO19/20（原生 USB CDC）；**UART0**：TX=GPIO43、RX=GPIO44。
  - **LDO**：ME6217C33M5G，Max 800mA。
- 页面“引出引脚”表 / “物理排布” ASCII 填实（USB 在顶、丝印正读、LED 面朝自己）。
- 新增 `examples/imu_qmi8658/`（I2C 扫描 + WHO_AM_I 探测，版本无关）。
- 原理图 PDF 入库后，Waveshare 该板的「引脚未核实」TODO 全部清零；残留：QMI8658 SA0(pins1) 接法（决定 0x6A/0x6B，按惯例按 0x6A 记）。

## [2025-07-21] update | Waveshare ESP32-S3-Matrix：QMI8658 地址定稿

- 用户确认原理图上 QMI8658C pin1(SA0) 接 **GND** → 7 位 I2C 地址 = **0x6A**（定稿，去掉 hedge）。
- 至此 Waveshare ESP32-S3-Matrix 全部引脚/地址 TODO 清零；仅剩"原理图 PDF 本地归档已完成"。

## [2025-07-21] fix | M5StickC Plus2：据官方 Plus2 文档+原理图 v0.5 全面更正（原误用 Plus 资料）

**背景**：原页面误用 StickC **Plus** 文档当 Plus2（以为二者引脚一致）。用户指出官方 Plus2 页面（路径含空格 M5StickC%20PLUS2）后，抓取权威资料 + 本地原理图，发现多处实质错误，已全面更正。

**关键修正（错→对）：**
- 主控：ESP32-PICO-D4 → **ESP32-PICO-V3-02**；Flash 4MB → **8MB**；**新增 2MB PSRAM**。
- 电源：AXP192 → **移除 AXP192**（PMIC 没了）；改用 SY8089(3V3)+SY7088(5V)+TP4057(充电)+BM8563(RTC)。
- 红色 LED：G10 → **G19（与 IR 发射管共用同一脚）**；另有绿色 LED（不可编程，睡眠指示）。
- IR：G9 → **G19**。
- TFT DC：G23 → **G14**；RST：G18 → **G12**；背光：经 AXP192 → **G27(GPIO)**。
- 新增按键 C(WAKE)=**G35**、HOLD=**G4**（开机后须置1）、电池电压检测=**G38(ADC)**。
- 电池：~200mAh(待核实) → **200mAh@3.7V 定稿**；USB-UART：**CH9102**（CP34X 驱动）。
- 删除误用的 m5stickc-plus_pinmap.txt（Plus 资料），新建权威 m5stickc-plus2_pinmap.txt。
- 示例 blink_led：LED_PIN 10 → **19**；platformio.ini 用官方 m5stack-stickc-plus2 board + 8MB/PSRAM + M5Unified。
- 原理图 PDF 已入库 raw/schematics/m5stack-stickc-plus2.pdf，交叉印证（CH9102F/无AXP/G37G39按键/mic G0G34/MPU6886@0x68）。

**确认无误：** 按键 A=G37 / B=G39（官方 Plus2 文档明确标注）。

**StickC Plus2 现已零 TODO。** 教训：同系列迭代版不一定引脚一致，必须按具体型号的官方资料+原理图。

## [2025-07-21] fix | Timer Camera F：据官方库源码+原理图厘清 BM8563 唤醒机制（IRQ 非 GPIO）

- 读官方库 m5stack/TimerCam-arduino 源码 + 用户读原理图，厘清定时唤醒：
  - timerSleep() → 设 BM8563 闹钟 → 拉低 POWER_HOLD(G33) 切主电 → ~2μA 近断电；到点 BM8563 ~INT(IRQ) 经网络 `RTC_ALM` **直接触发电源开关重新上电** → 冷启动。
  - **结论：BM8563 IRQ 不接任何 ESP32 GPIO**（官方引脚图无 IRQ 行，与此一致）。
  - 库核实：POWER_HOLD=G33、LED=G2、BAT_ADC=G38(ADC1_CH2)。
  - 外部唤醒(ext_wakeup.ino)：保持 HOLD 进 deep sleep，ext0 唤醒脚=GPIO4（HY2.0-4P 的 SDA 线复用）。
- BM8563 引脚接线（原理图）：pin3 ~INT→RTC_ALM、SDA→G12、SCL→G14、VDD→VBAT-IN。
- 删除自造的"背面 U 形焊盘"（官方文档无此说法，系之前臆测，已订正）。
- 新增 raw/datasheets/m5stack-timercam-f_library-power.txt（库源码摘录）。
- 残留（均属归档类，非数据缺口）：原理图 PDF / OV3660 datasheet 待归档入 raw/；OV3660 SCCB 7 位地址(0x3C)待核实（esp32-camera 会自动探测）。

## [2025-07-21] lint | 最终体检（三块板全部定稿后）

检查 10 项，结果：
- [1 链接] 67 个 md/pdf 链接，0 broken ✅
- [2 index 覆盖] 4 页全在目录，无悬空 ✅
- [3 反向链接] refs↔used_by 0 问题 ✅
- [4 芯片一致] index 表与各页 frontmatter 0 冲突 ✅
- [5 stale] 发现并修复：boards/README.md「已收录」仍写 StickC Plus2=ESP32-PICO-D4（旧值）→ 改为 ESP32-PICO-V3-02。StickC 页面里的 PICO-D4 是 Plus-vs-Plus2 对比表中的有意引用，保留。
- [6 孤儿页] 0 ✅
- [7 frontmatter] 4 页 type/必填字段齐全 ✅
- [8 examples] 所有示例都有 platformio.ini + src，页面引用的示例均存在 ✅
- [9 §8 不重复] OV3660 器件细节只在模组页，板子页无重复 ✅
- [10 TODO] 仅剩归档类（原理图/OV3660 datasheet 待存 raw/）+ OV3660 SCCB 地址 0x3C（待核实，驱动自动探测）+ index 里对比页候选（非缺口）。无数据缺口 ✅

结论：三块板 + OV3660 模组页全部定稿，零数据缺口，零断链。wiki 可用。

## [2026-07-21] ingest | Waveshare ESP32-S3-Matrix：bring-up 踩坑流水入库（字节序/像素映射/USB-Serial-JTAG/4MB boot loop）

- 原始资料：`raw/matrix-s3-onboarding-issues-2026-07-21.md`（从零点灯 + WiFi 配网小程序全流程踩坑，7 条「现象→根因→解法」）。
- 本条是「把实战经验回填 wiki」：原始流水自述「本文件只是踩坑流水，建议 ingest 进板子页」，故不新建页面，全部折叠进既有 `boards/waveshare-esp32-s3-matrix/README.md`。
- 新增/订正的板子事实（写入板子页）：
  - **WS2812 字节序 = RGB（非 GRB）** → 用 `NEO_RGB`（之前板子页未记；标定法=纯原色行，低亮度 HSV 会误导）。
  - **像素映射 = 行主序、非蛇形、idx0=左上角**（行↓列→）（之前未记）。
  - USB-C = ESP32-S3 **原生 USB-Serial-JTAG**，**无 USB-UART 桥**，VID:PID=`303A:1001`，板上**唯一串口通路**（之前仅写「原生 USB CDC」）。
  - 亮度：`setBrightness(b)` 降每通道占空比，`1` 为硬件下限，再暗需 app 时间抖动（会频闪）；与项目固件 LIT_VALUE=16 的取舍一致，板子页记为范围 1–~20 + 热警告。
- 「特殊说明」大幅扩充：新增 bring-up 踩坑表（坑 0–6 一句话摘要 + 指向 raw），含：
  - 坑1（关键）：stock `esp32-s3-devkitc-1` 实为 -N8(8MB)，镜像头写 8MB 在 4MB 板上 boot loop；`board_build.flash_size` 无效（elf2image 读 `upload.flash_size`）→ 需自定义 4MB board JSON。
  - 坑2：esptool stub 在 USB-Serial-JTAG 不稳 → `--no-stub` 烧录。
  - 坑3：pyserial 触发自动复位 → raw `os.open()` 读串口；`Serial` 须 HWCDC（`ARDUINO_USB_CDC_ON_BOOT=1`）。
- 一致性修正（lint 级）：
  - `examples/matrix_smile/src/main.cpp`：`NEO_GRB`→**`NEO_RGB`**（直接违反刚入库的字节序事实）；`setBrightness(20)`→`16` + 取舍注释；补像素映射注释。
  - `examples/matrix_smile/platformio.ini`：加 boot-loop 警告注释（`esp32-s3-devkitc-1` 在本板需自定义 board，否则重启循环）。
- frontmatter `sources` + 「参考来源」补 `raw/matrix-s3-onboarding-issues-2026-07-21.md`；index「最后更新」→ 2026-07-21。
- ⚠️ 日期备注：本 raw 源自带日期 **2026-07-21**，而既有 wiki 条目均为 2025-07-21。按时间线理解为「2025 据官方文档建页 → 2026 实战 bring-up 后回填」，故保留板子 `date_added=2025-07-21`、本条与 index 用 2026-07-21。若 2026 系笔误请告知订正。
- 残留：`examples/imu_qmi8658/platformio.ini` 与 matrix_smile 共用同一 `esp32-s3-devkitc-1`+`board_build.flash_size=4MB`，同样有 boot-loop 风险（本 raw 仅涉 LED，未改 IMU 示例）；是否同步加注释待定。

## [2026-07-23] ingest | ESP32-C3-OLED：自制板入库

- 新增 boards:
  - `boards/esp32-c3-oled/` — ESP32-C3，板载 SSD1306 128×64 OLED（I2C: SDA=GPIO5, SCL=GPIO6），USB-C，4MB Flash，11 GPIO 引出，BOOT/RESET 按键。
- 新增 modules:
  - `modules/ssd1306-oled/` — SSD1306 OLED 模组页，`used_by: [esp32-c3-oled]`
- 原理图 PDF 入库：`raw/schematics/esp32-c3-oled.pdf`
- 示例代码入库：`boards/esp32-c3-oled/examples/oled_hello_world/`（Adafruit SSD1306，Hello World + 计数器 + 堆内存监控）
- 原始 ino 文件保留在 `raw/c3-oled.ino`
- 更新 `index.md`（新增板 + 模组 + 交叉视图 ESP32-C3 分类 + 组合体关系）
- TODO（lint 候选）：
  - SSD1306 I2C 默认地址 0x3C 待核实（当前按原理图标注记）
  - ME6211C33 LDO 最大输出电流待核实（当前记 300mA）

## [2026-07-23] fix | 订正 ESP32-C3-OLED 录入日期（copy-paste 误写 2025-07-21 → 实际 2026-07-23）

- 系统日期确认今天 = 2026-07-23；c3-oled 相关文件 mtime 也是 7 月 23 日。
- 原录入把日期 copy-paste 成 2025-07-21（其他板 2025 年建页的真实日期），与实际不符。就地订正为 2026-07-23：
  - `boards/esp32-c3-oled/README.md`：frontmatter `date_added` + 参考来源 2 处「入库」日期。
  - `modules/ssd1306-oled/README.md`：frontmatter `date_added`。
  - `log.md`：c3-oled ingest 条目 header 日期（就地订正 + 本条记录留痕）。
  - `index.md`：「最后更新」。
- 不动其他板的 2025-07-21（那是 2025 年建 wiki 的真实日期；时间线自洽：2025 建页 → 2026 实战回填/新增）。
- matrix bring-up（raw 自带 2026-07-21）日期本来就对，不改。

## [2026-07-23] ingest | SSD1306 屏体规格书入库 + 订正分辨率（128×64 → 72×40，0.96" → 0.42"）

- 原始资料：`raw/c3-oled-screen.pdf`（Newvision **N042-7240TSWEG01-H16** 屏体规格书，Ver A / 2019-05-15）。
- **关键订正（冲突解决，以新源为准）**：c3-oled 录入时把 OLED 误记为「0.96" 128×64」（无来源的默认假设），据本规格书核实实际为 **0.42" 72×40**：
  - 分辨率 72×40；1/40 Duty；单色白；像素间距 0.128×0.13mm；有效区 9.196×5.18mm。
  - 驱动 IC 确为 **SSD1306**（不变；IC 可寻址到 128×64，但本面板物理 72×40，init mux=`0x27`=1/40）。
  - I2C 地址 **0x3C**（写 `0x78`，SA0=0）——**确认**，原「待核实」消除。
- 改动：
  - `modules/ssd1306-oled/README.md`：全面订正（0.42"/72×40），补面板型号、COG 16-pin 引脚表、电气特性（VDD 1.65–3.3V、VCC ~7.25V 内部 charge pump、-40~85°C、360cd/m²）；`sources` 补规格书；示例链接到板子页 oled_hello_world。
  - `boards/esp32-c3-oled/README.md`：frontmatter / 一句话 / 概览 / 表 A 共 4 处 128×64/0.96 → 72×40/0.42"。
  - `examples/oled_hello_world/src/main.cpp`：SCREEN 128×64 → **72×40**，重排布局以适配小屏（原 textSize 3「Hello/World」+ `drawRect(28,24,72,40)` 在 72×40 上会溢出）。
  - `index.md`：模块表标 72×40 + 最后更新。
- 残留 TODO：SSD1306 主控 IC datasheet（Solomon Systek）待入库（本规格书引用其 Technical Manual）。
- 提示：若 c3-oled 板实际焊的不是这块 72×40 屏（而是别的型号），告知订正；本次订正依据 = 屏体规格书 + 文件名 `c3-oled-screen.pdf`。

## [2026-07-23] fix | ESP32-C3-OLED / SSD1306 模组：标注单色

- 确认 SSD1306 为单色 OLED 驱动芯片（1-bit 像素，非彩色），board 页面 + module 页面 + 概览描述全部补上"白色单色"标注。
- 彩色 OLED 需 SSD1331/SSD1351 等驱动，SSD1306 不支持。

## [2026-07-23] ingest | ESP32-C3-OLED 实板反馈：排针计数、GPIO9 与 USB/UART 订正

- 原始资料：`raw/c3-boards.improve.20260723.md`（本地实板测试与文档复核反馈；按约定保持 raw 不变且不纳入仓库提交）。
- OLED 项已在前一轮规格书 ingest 中完成：页面与示例均为 **0.42" 72×40**，示例直接使用 72×40 画布，已无 128×64 虚拟画布及 28/24 像素偏移。
- 据原理图 JP1/JP2 核对排针：共 **16 个物理位置**，包含 3 个电源/地和 13 路 GPIO 信号；13 路为 GPIO0–10 加 GPIO20/21（UART0 RX/TX）。订正原“17 pin = 11 GPIO + 6 电源/功能”的矛盾，并解释“11”仅是不计 RX/TX 的标签数量。
- 强化 GPIO9 风险说明：GPIO9 同时是 strapping 引脚与 BOOT 按键网络（10K 上拉、按下接地），不再将 GPIO8/9 推荐为普通第二路 I2C；建议共享 OLED 的 GPIO5/6 总线，或优先映射到其他合适的空闲 GPIO。
- 据原理图订正 USB/UART 混淆：USB-C D-/D+ = **GPIO18/GPIO19 原生 USB Serial/JTAG**；GPIO20/GPIO21 = **UART0 RX/TX**。同时订正 Type-C CC1/CC2 为 5.1K 下拉到 GND。
- `examples/oled_hello_world/platformio.ini` 增加 `ARDUINO_USB_MODE=1` 与 `ARDUINO_USB_CDC_ON_BOOT=1`，使示例中的 Arduino `Serial` 通过 USB-C 原生 CDC 输出。
- 更新 `boards/esp32-c3-oled/README.md`、`index.md` 与本日志；无新增 TODO。
