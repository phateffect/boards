# modules/ — 模组小板

不能独立运行、要**插到核心板/底板上**用的小板（传感器模组、扩展模组、摄像头模组等）。每块一个子文件夹 `<vendor>-<slug>/`，里面：

- `README.md` — 模组 wiki 页面。固定结构：概览 / 核心器件 / **引脚定义** / 接口 / **引脚物理排布** / 示例代码 / 被哪些开发板使用 / 相关页面 / 参考来源。
- `examples/` — 驱动 / 点亮示例代码。

**这一层是器件信息的事实源**：同一种器件（如 OV3660）的引脚、接口、示例，只在它的模组页里详写一次；开发板用到时只引用过来。约定见 [`../CLAUDE.md`](../CLAUDE.md) §5/§6/§8。新模组从 [`_TEMPLATE/`](_TEMPLATE/) 复制起手。

## 已收录

> 完整目录见 [`../index.md`](../index.md)。当前收录 2 个：
>
> - [OV3660 Camera](ov3660-camera/README.md) — 器件事实源，被 [Timer Camera F](../boards/m5stack-timer-cameraf/README.md) 使用
> - [SSD1306 OLED](ssd1306-oled/README.md) — 0.96" 128×64 OLED 显示模组，被 [ESP32-C3-OLED](../boards/esp32-c3-oled/README.md) 使用
