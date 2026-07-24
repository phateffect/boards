# Boards Wiki

一个由 LLM 增量维护的**开发板 / 模组小板知识库**。

思路来自 LLM Wiki 模式：不要每次提问时都让 LLM 从原始资料里现查现拼（RAG 的做法），而是让 LLM **持续地、增量地维护一份结构化、互相链接的 markdown wiki**，它夹在你和原始资料之间。每加入一块新板子/一份新手册，LLM 不是简单地把原文索引起来，而是读懂它、提炼关键信息、把结果**整合进现有 wiki**——更新板子页面、刷新引脚表、补充交叉引用、标注新旧资料冲突、强化或修正已有的综合结论。知识被编译一次，之后**保持最新**。

**这个 wiki 是一个会复利的、持久的产物。** 交叉引用早就在那儿了，矛盾早就标注过了，综合结论已经反映了你读过的一切。每加一块板子、问一次问题，wiki 都变得更厚。

你几乎从不亲手写 wiki——**LLM 写、LLM 维护**。你负责选材（找板子、找手册、找原理图）、探索和提问；LLM 负责所有枯燥的活：总结、交叉引用、归档、保持一致。实践中：一边开着 LLM Agent（这里用 pi / Claude Code / Codex），一边开着 Obsidian 浏览结果、看 graph view、顺着链接读页面。**Obsidian 是 IDE，LLM 是程序员，wiki 是代码库。**

---

## 这个 wiki 整理什么

对每一块板子，我们至少要搞清楚并记下来：

- **名字**（厂商、型号、别名）
- **主控芯片**（ESP32 / ESP32-S3 / C3 / C6 / RP2040 …）
- **板载模组以及它们占用的引脚**（USB-UART、PSRAM、Flash、屏幕、传感器、TF 卡槽、LED、按键…以及各自吃掉了哪些 GPIO）
- **引出引脚 + 这些引脚的物理排布**（边缘排针 / 邮票孔 / Grove / JST / FPC，丝印到网络的映射，以及它们在板子上的物理位置）
- **原理图与示例代码**（如果有）

## 两类对象，分两个目录管

- **`boards/`（开发板）** — 能独立上电运行的板子。每块板一个子文件夹，里面放：
  - `README.md`：板子的 wiki 页面（上面那些信息）
  - `examples/`：示例工程（PlatformIO `platformio.ini` + `src/`）
- **`modules/`（模组小板）** — 不能独立运行、要插到核心板/底板上用的小板（传感器模组、扩展模组等）。每块一个子文件夹，主要放：
  - `README.md`：引脚定义、器件、接口、物理排布
  - `examples/`：驱动 / 点亮示例代码

## 特殊情况：核心板 + 模组的组合

有些“开发板”其实是 **核心板 + 某块模组** 的组合体，例如 **M5Stack Timer CameraF = ESP32 + OV3660 摄像头小板**。

处理方式：**把它当作开发板管**（在 `boards/` 下建页面，写主控、引脚、板载占用等），但**不重复写 OV3660 那部分**——只在页面里说明“使用了 OV3660”，并 **链接到 `modules/` 下对应的模组页面**，点过去就能看到完整信息。详见 [CLAUDE.md §8](CLAUDE.md)。

---

## 架构（三层）

**Raw sources（原始资料）** — `raw/`，你整理来的数据手册、原理图、板子照片、官方文档、示例代码。**不可变**，LLM 只读不改。这是事实来源。

**The wiki** — LLM 生成并维护的 markdown：`index.md`、`log.md`，以及 `boards/`、`modules/` 下每块板/模组的 `README.md`。**这一层完全归 LLM**——它建页面、来新资料时更新、维护交叉引用、保持全局一致。你读它；LLM 写它。

**The schema** — [`CLAUDE.md`](CLAUDE.md)（pi / Claude Code）或 `AGENTS.md`（Codex）。告诉 LLM wiki 怎么组织、约定是什么、ingest/query/lint 时该做什么。这是让 LLM 变成“靠谱的 wiki 维护者”而非“通用聊天机器人”的关键配置。你和 LLM 会随着使用逐步 co-evolve 这份文件。

## 操作（Operations）

**Ingest（录入）。** 把一份新资料（手册 PDF、官网页面、原理图、照片）丢进 `raw/`，让 LLM 处理。典型流程：LLM 读资料 → 跟你过一遍要点 → 在 wiki 里写/更新对应板子的页面（更新引脚表、占用表、交叉引用、index、log）。一份好资料可能改动 5–15 个文件。建议**一块一块录入并保持参与**：读一读摘要、检查更新、引导 LLM 强调什么。也可以批量录入少监督，看你的风格。

**Query（查询）。** 对 wiki 提问。LLM 先读 `index.md` 找相关页面，再钻进去读，综合出一个带引用的答案。**好的答案应该被归档回 wiki**——你问出来的一张对比表、一段分析、发现的一个关联，都是有价值的，别让它消失在聊天记录里。

**Lint（体检）。** 定期让 LLM 给 wiki 做健康检查：页面之间有没有矛盾、有没有被新资料推翻的过时结论、有没有没人链进来的孤儿页面、被反复提到却没单独成页的概念、缺失的交叉引用、可以补的数据缺口（可能一次网搜就能补上）。详见 [CLAUDE.md §11 lint](CLAUDE.md)。

## 索引与日志

两个特殊文件帮助在 wiki 变大后依然好导航：

- **[`index.md`](index.md)** — 面向内容的目录。每个页面一行：链接 + 一句话摘要 + 元数据（芯片、接口、被谁用等），按类别组织。LLM 每次 ingest 都更新它。回答问题时 LLM 先读 index 找页面，再钻进去读。中等规模（上百个页面）下效果很好，不需要向量 RAG。
- **[`log.md`](log.md)** — 面向时间的流水账。追加记录每次 ingest/query/lint。约定每条以 `## [YYYY-MM-DD] <op> | <标题>` 开头，这样能用简单 unix 工具查询：`grep "^## \[" log.md | tail -10` 给你最近 10 条。日志让你能看到 wiki 的演化时间线，也帮 LLM 知道最近干过什么。

## 工具小贴士

- **Obsidian** 用来浏览 / graph view / 跟链接。本 wiki 用 **相对 markdown 链接**（`../modules/xxx/README.md`）做交叉引用，在 GitHub / VS Code / Obsidian / 纯 markdown 阅读器里都能点开；Obsidian graph view 也会把它们画进去。
- **下载图片到本地。** Obsidian 设置 → Files and links，把 Attachment folder path 设成 `raw/assets/`。手册里的引脚图、板子照片存本地后，LLM 能直接看图、引用，不必依赖会失效的 URL。
- **Dataview**（Obsidian 插件）可以基于页面 frontmatter 跑查询。LLM 给页面写 YAML frontmatter（type/vendor/chip/interface/tags…），Dataview 就能生成动态表。
- 整个 wiki 就是**一个 markdown + git 仓库**，版本历史 / 分支 / 协作都免费。

## 目录结构

```
boards/                         # 仓库根
├── README.md                   # 你在这
├── CLAUDE.md                   # schema（LLM 维护规则）
├── index.md                    # 内容目录（按类别）
├── log.md                      # 时间流水账
├── raw/                        # 原始资料，不可变
│   ├── datasheets/             # 数据手册（PDF 等）
│   ├── schematics/             # 原理图
│   ├── photos/                 # 板子照片
│   └── assets/                 # wiki 页面引用的图片
├── boards/                     # 开发板，每块一个子文件夹
│   ├── README.md               # 本节组织说明
│   ├── _TEMPLATE/              # 板子页面模板
│   └── <vendor>-<board>/
│       ├── README.md           # 板子 wiki 页面
│       └── examples/           # PlatformIO 示例
└── modules/                    # 模组小板，每块一个子文件夹
    ├── README.md               # 本节组织说明
    ├── _TEMPLATE/              # 模组页面模板
    └── <vendor>-<module>/
        ├── README.md           # 模组 wiki 页面
        └── examples/           # 驱动示例
```

## 为什么这套管用

维护知识库累人的地方不在读、不在想，而在**簿记**：更新交叉引用、保持摘要不过时、标注新旧矛盾、在几十个页面间保持一致。人放弃 wiki，是因为维护成本增长得比价值还快。LLM 不会嫌烦、不会忘更新一个交叉引用、能一次改动 15 个文件。**wiki 之所以能保持维护，是因为维护成本接近于零。**

人的活：选材、定向、问好问题、想清楚这意味着什么。
LLM 的活：剩下的一切。

---

## 发现文档有问题？

wiki 内容有错漏、过期、矛盾，或想补充内容，欢迎提 issue。命令行里：

```bash
gh issue create --repo phateffect/boards
```

或直接在 GitHub 上 [新建 issue](https://github.com/phateffect/boards/issues/new)。
