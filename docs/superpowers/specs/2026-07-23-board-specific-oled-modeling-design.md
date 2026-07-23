# ESP32-C3-OLED 板载屏建模调整设计

日期：2026-07-23

## 背景

当前 wiki 将板载 Newvision N042-7240TSWEG01-H16 屏体建模为通用
`ssd1306-oled` 模组。这个名称只表达驱动 IC，无法区分使用 SSD1306
但尺寸、分辨率、接口板和引脚均不同的其他显示模组。

该屏体目前仅属于 ESP32-C3-OLED 板，且不是仓库中可跨开发板复用的
独立小板。因此继续保留通用 `ssd1306-oled` 页面会造成错误归类。

## 决策

删除 `modules/ssd1306-oled/`，把该屏体与本板相关的必要信息归并到
`boards/esp32-c3-oled/README.md`。

板子页保留以下事实：

- 屏体厂商与型号：Newvision N042-7240TSWEG01-H16。
- SSD1306 驱动 IC、0.42 英寸、72×40、白色单色。
- I2C 地址 `0x3C`，SDA=GPIO5，SCL=GPIO6。
- 板级使用所需的电源、初始化和占用引脚信息。
- `raw/c3-oled-screen.pdf` 作为屏体规格来源。

COG 16-pin 等只对屏体封装有意义、对开发板使用无直接帮助的细节不迁移，
避免把板子页变成通用 SSD1306 数据手册。

## 仓库变更

1. 从板子 frontmatter 删除 `references_modules: [ssd1306-oled]`，改为空列表。
2. 把 `raw/c3-oled-screen.pdf` 加入板子页 `sources`。
3. 将必要屏体规格合并进板子页的板载模组、特殊说明和参考来源。
4. 删除 `modules/ssd1306-oled/README.md` 及其目录。
5. 从 `index.md` 的模组表和组合体关系中删除该模组，并将 ESP32-C3-OLED
   标为非组合体。
6. 更新 `modules/README.md` 的收录数量与清单。
7. 在 `log.md` 追加一次修正记录，不改写历史 ingest 条目。

示例工程保留在板子目录中；代码和屏幕参数不需要改变。

## 验证

- 全仓库不存在指向 `modules/ssd1306-oled` 的引用。
- 所有相对 Markdown 链接均可解析。
- board/module frontmatter 的 `references_modules` 与 `used_by` 无残留关系。
- `index.md`、`boards/README.md`、`modules/README.md` 的数量和分类一致。
- `git diff --check` 通过。
