# boards/ — 开发板

能**独立上电运行**的板子。每块一个子文件夹 `<vendor>-<slug>/`，里面：

- `README.md` — 板子的 wiki 页面。固定结构：概览 / 主控芯片 / **板载模组与占用引脚** / **引出引脚** / **引脚物理排布** / 原理图 / 示例代码 / 特殊说明 / 相关页面 / 参考来源。
- `examples/` — PlatformIO 示例工程（`platformio.ini` + `src/`）。

录入流程、引脚表示约定、特殊规则全在 [`../CLAUDE.md`](../CLAUDE.md)。新板子从 [`_TEMPLATE/`](_TEMPLATE/) 复制起手。

## 特殊情况

板子若是 **核心板 + 模组** 的组合体（如 Timer CameraF = ESP32 + OV3660 小板）：仍在这里建页面，但模组那部分**只引用** [`../modules/`](../modules/) 下的模组页，不重复详写。见 CLAUDE.md §8。

## 已收录

> 完整目录见 [`../index.md`](../index.md)。当前收录 3 块：
>
> - [M5StickC Plus2](m5stack-stickc-plus2/README.md) — ESP32-PICO-D4
> - [Waveshare ESP32-S3-Matrix](waveshare-esp32-s3-matrix/README.md) — ESP32-S3
> - [M5Stack Timer Camera F](m5stack-timer-cameraf/README.md) — ESP32 + OV3660（组合体）
