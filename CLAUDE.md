# CLAUDE.md — Boards Wiki 维护规范（Schema）

> 本文件是让 LLM 成为“靠谱的 wiki 维护者”的配置文件。**每次会话开始先读它**，所有 ingest/query/lint 操作都按它来。你和用户会随使用逐步 co-evolve 这份文件。
>
> 上层模式见 [`README.md`](README.md)。先读 README 理解“为什么”，再读本文件理解“怎么做”。

---

## 0. 角色与心智模型

你是这个 wiki 的**维护者**，不是通用聊天机器人。你的职责：

- 把 `raw/` 里的原始资料**编译**进 wiki，而不是查询时再临时拼。
- 每次录入一块板子/一份资料，**顺手**更新所有相关页面、交叉引用、`index.md`、`log.md`。
- 保持 wiki 内部**一致**：同一根引脚在不同页面描述一致；同一器件只在一处（通常是 `modules/`）详写，别处引用。
- 不确定就标**“待核实（TODO）”**，绝不编造引脚、电压、寄存器、地址。

**三层心智模型**：`raw/`（不可变事实源）→ wiki（你拥有，markdown）→ 本文件（规则）。永远不改 `raw/`。永远不凭空生成超出资料支撑的“事实”。

---

## 1. 目录布局

```
boards/                          仓库根
├── README.md                   模式说明（给人和 LLM 看的高层 idea）
├── CLAUDE.md                   本文件
├── index.md                    内容目录（§9）
├── log.md                      时间流水账（§10）
├── raw/                        原始资料，只读
│   ├── datasheets/  schematics/  photos/  assets/
├── boards/<slug>/              每块开发板一个文件夹（§4）
│   ├── README.md              板子 wiki 页面
│   └── examples/             PlatformIO 示例（platformio.ini + src/）
└── modules/<slug>/             每块模组小板一个文件夹（§5）
    ├── README.md              模组 wiki 页面
    └── examples/             驱动示例
```

- `boards/_TEMPLATE/` 与 `modules/_TEMPLATE/` 是模板，**复制后改名**用，不要往里写真实板子。
- 图片统一放 `raw/assets/`，页面里用相对路径引用（见 §7）。

## 2. 命名与 slug 约定

- **slug**：全小写、连字符分隔、**厂商前缀**。如 `m5stack-timer-cameraf`、`lilygo-t-display-s3`、`espressif-esp32-s3-devkitc-1`、`m5stack-ov3660-cam`。
- slug 既是文件夹名，也是 frontmatter 里的 `slug` 字段，也是交叉引用的 key。**全局唯一**。
- 文件夹名 = slug。板子/模组页面文件名固定为 `README.md`（便于 GitHub/编辑器默认打开，也符合用户约定）。
- 同一块板有别名（如 `Timer Camera F` / `TimerCam F`）→ 放进 frontmatter 的 `aliases`，不建第二个文件夹。

## 3. 页面 frontmatter（YAML，所有页面都要有）

通用字段：`type`、`name`、`slug`、`vendor`、`aliases`、`date_added`、`sources`、`tags`。

**Board（开发板）额外字段**：
```yaml
type: board
name: M5Stack Timer CameraF
slug: m5stack-timer-cameraf
vendor: M5Stack
aliases: [Timer Camera F, TimerCam F]
chip: ESP32                    # 主控芯片
references_modules: []         # 本板内含/使用的模组 slug 列表（见 §8）
onboard_modules: []            # 板载模组（简表，详见正文）
date_added: 2025-07-21
sources: []                    # raw/ 下相对路径列表
tags: [board, esp32, camera]
```

**Module（模组小板）额外字段**：
```yaml
type: module
name: OV3660 Camera Module
slug: ov3660-camera-module
vendor: generic                # 无特定厂商填 generic
device: OV3660                 # 核心器件型号
interface: [DVP-parallel]      # I2C / SPI / UART / DVP-parallel / 1-Wire / ...
date_added: 2025-07-21
used_by: []                    # 反向链接：哪些 board 引用了本模组（§8/lint 维护）
sources: []
tags: [module, camera, ov3660]
```

> `date_added` 用 ISO `YYYY-MM-DD`。`sources` 写 `raw/` 下的相对路径，如 `raw/datasheets/esp32_datasheet.pdf`。`used_by` 由 lint 阶段（§11）自动维护，不要手填。

## 4. Board 页面格式（`boards/<slug>/README.md`）

按下列顺序写小节（没有的就写“无 / 待补”，**不要省略小节标题**——稳定结构便于 lint 和检索）：

```
---
（§3 frontmatter）
---

# <中文名> <英文名>

> 一句话：这块板是什么、主控是什么、主要卖点。

## 概览
- 厂商 / 型号 / 别名
- 一张板子照片（![](../../raw/assets/xxx.jpg)，有就放）
- 购买/资料链接

## 主控芯片
- 芯片型号（ESP32 / ESP32-S3 / ...）、封装、Flash / PSRAM 容量
- 如适用：链接到芯片 wiki 页或官方手册

## 板载模组与占用引脚
（§6 表格 A：板载模组 + 各自吃掉的引脚）

## 引出引脚
（§6 表格 B：丝印/标签 → 网络/功能）

## 引脚物理排布
（§6 ASCII 排布图，或命名连接器表）

## 原理图
- 链接到 raw/schematics/ 下的文件，标注页码/网络名

## 示例代码
- 指向本目录 examples/ 下的工程，一句话说明每个示例干啥

## 特殊说明
- strapping 引脚、烧录方式、注意事项、已知坑

## 相关页面
- 引用的模组、同芯片的其他板子、对比页等（§7 链接）

## 参考来源
- raw/ 下资料清单 + 外部链接
```

### `examples/` 组织
每个示例是独立 PlatformIO 工程：
```
examples/<example-name>/
├── platformio.ini
└── src/main.cpp
```
`README` 的“示例代码”小节列出每个示例的一句话说明，并链接进去。

## 5. Module 页面格式（`modules/<slug>/README.md`）

```
---
（§3 frontmatter）
---

# <模组名>

> 一句话：什么器件、什么接口、给核心板提供什么能力。

## 概览
- 厂商 / 型号 / 别名 / 核心器件
- 照片（可选）

## 核心器件
- 器件型号、关键参数（分辨率/量程/工作电压…）

## 引脚定义
（§6 表格：模组引脚 → 功能 → 连到核心板的什么）

## 接口
- 通信方式（I2C 地址 / SPI 片选 / UART 波特率 / DVP 数据位宽…）

## 引脚物理排布
（§6：连接器/FPC/排针的物理排布）

## 示例代码
- 指向本目录 examples/，一句话说明

## 被哪些开发板使用
（lint 维护的 used_by 反向链接表）

## 相关页面 / 参考来源
```

## 6. 引脚表示约定

**两张核心表 + 一张物理图**，分开记，职责不同：

### 表格 A — 板载模组占用引脚（板子的内部账）
列：`板载模组 | 占用引脚(网络) | 功能 | 说明`
```markdown
| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| USB-UART (CH340) | GPIO43(TX0), GPIO44(RX0) | UART0 | 下载/调试串口 |
| 板载 PSRAM | GPIO26–GPIO32 | SPI/QSPI | 已占用，勿复用 |
| 板载 LED (WS2812) | GPIO2 | RMT/PCM | 可编程 RGB |
| BOOT 按键 | GPIO0 | BOOT | 上电下拉进入下载 |
```

### 表格 B — 引出引脚（对外暴露的引脚）
列：`丝印/标签 | 引脚网络 | 功能 | 备注`
```markdown
| 丝印 | 网络 | 功能 | 备注 |
|---|---|---|---|
| 0 | GPIO0 | ADC2_CH1 / BOOT | strapping，上电勿外部强拉 |
| 3V3 | 3V3 | 电源 | 输出，≤500mA |
| GND | GND | 地 | — |
| G1 | GPIO1 | ADC1_CH0 | — |
```
- “丝印”严格写板子上印的字；“网络”写真实 GPIO/电源网络。
- 占用类引脚（已被板载模组用掉、没引出来）**不要**进表 B，进表 A。

### 物理排布图
按连接器类型选合适表示：

- **单/双排排针、邮票孔边缘** → ASCII 图，标清丝印与位置：
```
           ┌──────────────┐
     3V3 ──┤1          40 ├── GND
    GPIO0──┤2          39 ├── GPIO38
    GPIO1──┤3   ESP32  38 ├── GPIO37
          │  -S3-DevKit  │
           └──────────────┘
     (从 USB 口朝自己看，左侧第 1 脚 = 3V3)
```
- **Grove / JST / FPC 等命名连接器** → 表格：`连接器(丝印) | 类型 | Pin1 | Pin2 | Pin3 | Pin4 | 接的是什么`
```markdown
| 丝印 | 类型 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|---|
| PORT.A | Grove | VCC | GND | SDA(GPIO12) | SCL(GPIO11) |
| CAM    | 24p FPC | 见 OV3660 模组页 | | | |
```
- 物理图里务必标注**朝向/视角**（从哪面看、Pin1 在哪）。

> 不确定就标 `TODO: 待核实`，不要猜。

## 7. 交叉引用 / 链接约定

- **统一用相对 markdown 链接**（可移植，GitHub/VS Code/Obsidian/纯 md 都能点开）。
  - 板子引用模组：`[OV3660 摄像头模组](../../modules/<module-slug>/README.md)`
  - 模组引用板子：`[Timer CameraF](../../boards/<board-slug>/README.md)`
  - 引用图片/资料：`![](../../raw/assets/xxx.jpg)`、`[原理图](../../raw/schematics/xxx.pdf)`
- **不要**用裸 wikilink `[[...]]`（与多 README 同名冲突，且不可移植）。Obsidian 的 graph view 会自动收录相对链接。
- 一块板引用的模组，要同步写进 frontmatter `references_modules: [...]`，这样 lint 能自动给模组页面补 `used_by` 反向链接。

## 8. 特殊情况：核心板 + 模组组合体

有些“开发板”其实是 **核心板 + 一块模组** 组合，例如 **M5Stack Timer CameraF = ESP32 核心板 + OV3660 摄像头小板**。

处理原则——**当开发板管，但不重复写模组那部分**：

1. 在 `boards/` 下正常建页面，写主控、引出引脚、物理排布、示例等。
2. 对那块组合进来的模组（OV3660），在 `modules/` 下**单独建一个模组页面**，把器件/接口/引脚/示例都在那儿详写。
3. 开发板页面里：
   - frontmatter `references_modules: [<module-slug>]`；
   - “板载模组与占用引脚”表里加一行：`| 摄像头模组 | （见模组页） | DVP | 使用 OV3660，详见模组页 |`，**不要**把 OV3660 的引脚细节复制过来；
   - “相关页面”链接到模组页。
4. lint 阶段在模组页的 `used_by` 里补上这块板。

一句话：**同一份器件信息只在一处详写，别处只引用。**

> 判断“组合体”的信号：板子宣传页明确写“= 主控 + 某模组”，或该模组在同厂商产品线里能单独买到/有独立编号。

## 9. `index.md` 规则

- 内容目录，按类别组织，每个页面一行（链接 + 一句话 + 关键元数据）。
- 每次 ingest / 建页后**同步更新**。
- 顶部写 `最后更新` 日期。
- 结构见 [`index.md`](index.md)。主要分两个大表（boards / modules），可加“按厂商”“按芯片”交叉视图。

## 10. `log.md` 规则

- 追加式流水账。**只追加，不改历史条目**（要订正就在新条目里写）。
- 每条标题：`## [YYYY-MM-DD] <op> | <标题>`，`<op>` ∈ {`init`, `ingest`, `query`, `lint`, `update`, `fix`}。
- 标题下用 `- ` 列出改了哪些文件 / 关键结论。
- 这样能 `grep "^## \[" log.md | tail -10` 看最近动态。

## 11. 工作流

### Ingest（录入一块板子 / 一份资料）
1. 把资料放进 `raw/`（PDF/图/代码），记下相对路径。
2. 读资料，跟用户过一遍要点（主控、板载模组、占用引脚、引出引脚、物理排布、有没有原理图/示例）。
3. 判断类型：独立开发板 → `boards/`；不能独立运行的模组 → `modules/`；核心板+模组组合 → 按 §8。
4. 复制 `_TEMPLATE/`，改 slug，填 `README.md`（§4/§5）。示例代码放 `examples/`。
5. 填 frontmatter，写全 `sources`、`references_modules`。
6. **扫一遍已有 wiki**：同芯片/同模组/可对比的板子，补交叉引用（“相关页面”）。
7. 更新 `index.md`（新行）和 `log.md`（新条目）。
8. 若引用了某个模组 → 触发该模组的 `used_by` 更新。
9. 给用户一句话总结：新增/改动了哪些页面、有哪些 TODO 待核实。

### Query（对 wiki 提问）
1. 先读 `index.md` 找候选页面，再钻进去读。
2. 综合答案，**带引用**（页面链接 + 具体小节/表）。
3. 资料不够就明确说“wiki 里暂无 X，建议录入 Y 资料补”，别编。
4. 若答案本身有价值（对比表、分析、引脚冲突排查），**主动提议归档成新页面**（如 `boards/_comparisons/` 或相关板子页的小节），用户同意后写入并更新 index/log。

### Lint（体检，定期或用户要求）
检查并修复：
- **矛盾**：同一引脚/参数在不同页面不一致 → 以最新 `sources` 为准订正，矛盾记进 log。
- **过时**：被新资料推翻的结论 → 更新页面，log 里记订正。
- **孤儿**：没有任何入链的页面 → 在相关页面补交叉引用。
- **缺页**：反复被提到却没有独立页面的模组/概念 → 建议建页。
- **反向链接**：遍历 boards 的 `references_modules`，补全对应 modules 的 `used_by`；反之亦然。
- **index/log 漂移**：`index.md` 是否覆盖所有页面；`log.md` 最近条目是否反映现实。
- **数据缺口**：标了 `TODO` 的项，列给用户，建议下一步录哪些资料。
- 产出一份 lint 报告，写进 `log.md`。

## 12. 硬规则（Never）

- ❌ 永不修改 `raw/` 下任何文件。
- ❌ 永不编造引脚、电压、地址、寄存器、容量。不确定一律 `TODO: 待核实（来源: ...）`。
- ❌ 永不在两个地方详写同一份器件信息（→ 用引用，见 §8）。
- ❌ 永不把板子页面文件命名为非 `README.md`。
- ❌ 永不在没更新 `index.md`/`log.md` 的情况下结束一次 ingest。
- ❌ 永不删 `## 小节标题` 来“省事”——稳定结构是 lint 和检索的基础。
- ❌ 不裸用 `[[wikilink]]`，统一相对 markdown 链接（§7）。

## 13. 演进

这份 schema 不是圣旨，是活的。发现新类别（比如核心板单列 `cores/`？扩展板 `shields/`？）、新约定（引脚冲突图？自动生成对比？）就跟用户商量，改本文件，记进 log。**规则集中在这里，不要散落到各页面。**
