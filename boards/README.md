# boards/ — 开发板

能**独立上电运行**的板子。每块一个子文件夹 `<vendor>-<slug>/`，里面：

- `README.md` — 板子的 wiki 页面。固定结构：概览 / 主控芯片 / **板载模组与占用引脚** / **引出引脚** / **引脚物理排布** / 原理图 / 示例代码 / 特殊说明 / 相关页面 / 参考来源。
- `examples/` — PlatformIO 示例工程（`platformio.ini` + `src/`）。

录入流程、引脚表示约定、特殊规则全在 [`../CLAUDE.md`](../CLAUDE.md)。新板子从 [`_TEMPLATE/`](_TEMPLATE/) 复制起手。

## 特殊情况

板子若是 **核心板 + 模组** 的组合体（如 Timer CameraF = ESP32 + OV3660 小板）：仍在这里建页面，但模组那部分**只引用** [`../modules/`](../modules/) 下的模组页，不重复详写。见 CLAUDE.md §8。

## 已收录

> 完整目录见 [`../index.md`](../index.md)。当前收录 9 块：
>
> - [M5StickC Plus2](m5stack-stickc-plus2/README.md) — ESP32-PICO-V3-02（8MB Flash + 2MB PSRAM）
> - [M5Stack StickS3](m5stack-sticks3/README.md) — ESP32-S3-PICO-1-N8R8，LCD / IMU / 音频 / 红外
> - [M5Stack StopWatch](m5stack-stopwatch/README.md) — ESP32-S3R8，1.75" 圆形 AMOLED / 触摸
> - [ESP32-S3 SuperMini](esp32-s3-supermini/README.md) — ESP32-S3FH4R2，4MB Flash + 2MB PSRAM
> - [ESP32-S3-AI-Board](esp32-s3-ai-board/README.md) — ESP32-S3-WROOM-1-N16R8，LCD / 数字麦克风 / I2S 音频
> - [Waveshare ESP32-S3-Matrix](waveshare-esp32-s3-matrix/README.md) — ESP32-S3
> - [M5Stack Timer Camera F](m5stack-timer-cameraf/README.md) — ESP32 + OV3660（组合体）
> - [ESP32-C3-OLED](esp32-c3-oled/README.md) — ESP32-C3 + 板载 Newvision 0.42" 72×40 OLED
> - [M5Stack Stack-chan](m5stack-stackchan/README.md) — CoreS3（ESP32-S3，16MB/8MB）+ 机器人本体（舵机/NFC/触摸/IR，组合体）
