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
