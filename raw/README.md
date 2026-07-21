# raw/ — 原始资料（只读）

你整理来的**事实来源**，LLM 只读不改。建议按类型分子目录：

- `datasheets/` — 数据手册（芯片、模组的 PDF）
- `schematics/` — 板子原理图
- `photos/` — 板子 / 模组照片
- `assets/` — wiki 页面里引用的图片（引脚图、引脚分布图、连接器照片等）

> 习惯：从网页 / 手册抓图片时，统一存到本地（Obsidian 可设 Attachment folder = `raw/assets/`），wiki 里用相对路径引用，避免外链失效。

## 命名

建议 `<vendor>-<slug>-<用途>.<ext>`，如 `m5stack-timer-cameraf-schematic.pdf`、`esp32-s3-pinout.png`。和 wiki slug 对齐，便于检索。

## 不可变

**不要改这里的文件**（重命名 / 重排可接受，但不要改内容）。要订正信息 → 改 wiki 页面，并在 log 里记一笔。
