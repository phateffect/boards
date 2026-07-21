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
