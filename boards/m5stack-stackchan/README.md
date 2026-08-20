---
type: board
name: M5Stack StackChan
slug: m5stack-stackchan
vendor: M5Stack
aliases: [Stack-chan, StackChan, SKU K151, SKU K151-R]
chip: ESP32-S3
references_modules:
  - m5stack-stackchan-body
onboard_modules:
  - CoreS3 主控（2.0" ILI9342C 触摸屏 / FT6336U / GC0308 摄像头 / LTR-553ALS-WA / BMI270+BMM150 / ES7210 双麦克风 / AW88298 1W 功放 / AXP2101 / BM8563 / microSD / Grove×3）
  - 机器人本体模组 → 见 modules/m5stack-stackchan-body（SCS0009 舵机×2 / WS2812C×12 / IR 收发 / ST25R3916 NFC / Si12T 三区触摸 / INA226 电量计 / PY32L020 扩展 / 550mAh 电池）
date_added: 2026-08-20
sources:
  - raw/datasheets/m5stack-stackchan-doc.txt
  - raw/datasheets/m5stack-cores3-doc.txt
  - raw/schematics/m5stack-stackchan-sch-adapter.pdf
  - raw/schematics/m5stack-stackchan-sch-power.pdf
  - raw/schematics/m5stack-stackchan-sch-ring.pdf
  - raw/schematics/m5stack-stackchan-sch-touch.pdf
  - raw/schematics/m5stack-cores3-v1.0.pdf
  - raw/datasheets/m5stack-stackchan-model-size.pdf
  - raw/assets/m5stack-stackchan-main.webp
tags: [board, esp32-s3, m5stack, robot, servo, nfc, xiaozhi, combo]
---

# M5Stack Stack-chan（StackChan）

> M5Stack 与 Stack-chan 社区（[@meganetaaan](https://x.com/meganetaaan)、[@mongonta555](https://x.com/mongonta555)）共创的桌面 AI 机器人套件：**CoreS3 主控（ESP32-S3，16MB Flash / 8MB PSRAM）+ 机器人本体**（双反馈舵机、12×RGB、红外收发、全功能 NFC、顶部三区触摸、550mAh 电池）。出厂固件内置小智（XiaoZhi）驱动的 AI Agent、Avatar 头像联动、ESP-NOW 遥控、在线 App 商店与 OTA。

> 📌 **核心板 + 模组组合体**：整机 = CoreS3（主控，官方另有独立产品页）+ 机器人本体。本体器件（舵机/RGB/IR/NFC/触摸/电量计/IO 扩展）细节**只在**[本体模组页](../../modules/m5stack-stackchan-body/README.md)详写；CoreS3 板内器件的引脚占用官方在 CoreS3 页维护，本 wiki 暂无 CoreS3 独立页（TODO，见「相关页面」）。

![Stack-chan 整机](../../raw/assets/m5stack-stackchan-main.webp)

## 概览

- 厂商 / 型号：M5Stack / StackChan，**SKU K151**（标准版）/ **K151-R**（遥控器套装）
- K151-R 附遥控器 = **StickC-Plus + Hat Mini JoyC** 组装，经 **ESP-NOW** 遥控（同频道通信，支持单播/广播/同 ID 群控）
- 官方文档：<https://docs.m5stack.com/en/StackChan>
- 整机尺寸 / 重量：54.0 × 70.5 × 61.5 mm / 187.2 g（遥控器 37.6 g）
- 定位：桌面陪伴 / AI 语音问答 Agent / 智能家居 / IoT 控制
- 出厂固件功能：AI Agent（唤醒词 "Hi, StackChan"，App 配置模型/音色/人设/记忆，支持 Home Assistant MCP）、Avatar（手机端头像/监控/动作/舞蹈）、ESP-NOW 遥控、App Center 在线应用、设置（含舵机标定）、OTA；App 为 StackChan World（iOS / Android）

## 主控芯片

- **ESP32-S3**（Xtensa LX7 双核 240MHz，Wi-Fi 2.4GHz 802.11 b/g/n + BLE 5）
- **16MB Flash + 8MB PSRAM**（官方规格表标注 Quad PSRAM）
- 搭载于 **M5Stack CoreS3** 主控（K128）：板内含 AXP2101 PMIC（0x34）、BM8563 RTC（0x51）、AW9523B IO 扩展（0x58）
- ESP32-S3 技术参考手册：<https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/490/esp32-s3_technical_reference_manual_en.pdf>

## 板载模组与占用引脚

> 表 A：整机内部账。CoreS3 侧器件中，**摄像头接线已转录**（见下方小节）；其余 CoreS3 板内器件（LCD/触摸/音频/SD）接线官方维护在 [CoreS3 PinMap](https://docs.m5stack.com/en/core/CoreS3)（快照已存 `raw/datasheets/m5stack-cores3-doc.txt`，待 CoreS3 独立页建成后迁过去）。

| 板载模组 | 占用引脚 | 功能 | 说明 |
|---|---|---|---|
| 2.0" IPS LCD（ILI9342C） | 见官方 CoreS3 PinMap | 320×240 显示 | CoreS3 板内 |
| 电容触摸（FT6336U） | 见官方 CoreS3 PinMap | I2C 0x38 | CoreS3 板内 |
| 摄像头 GC0308 | SCCB=G12/G11（共内部 I2C）· PCLK=G45 · VSYNC=G46 · HREF=G38 · D0–D7=G39/G40/G41/G42/G15/G16/G48/G47 | DVP 640×480 0.3MP，I2C 0x21 | CoreS3 板内；与 LTR-553 同一条排线；XCLK/PWDN 未接，复位经 AW9523B；全表见下方小节 |
| 光距感 LTR-553ALS-WA | 内部 I2C（0x23） | 接近+环境光 | CoreS3 板内 |
| 9 轴 IMU BMI270(0x69)+BMM150(0x10) | 内部 I2C | 加计+陀螺+地磁 | 磁体会干扰 BMM150 |
| 双麦克风 ES7210（0x40） | 见官方 CoreS3 PinMap | I2S 音频编码 | CoreS3 板内 |
| 1W 扬声器功放 AW88298（0x36） | 见官方 CoreS3 PinMap | I2S 功放 | CoreS3 板内 |
| microSD 卡槽 | 见官方 CoreS3 PinMap | SDIO/SPI | CoreS3 底部 |
| PMIC AXP2101（0x34）/ RTC BM8563（0x51）/ AW9523B（0x58） | 内部 I2C | 电源/时钟/IO 扩展 | CoreS3 板内 |
| **机器人本体模组** | **G5(IR_SEND) · G6(Servo_TX) · G7(Servo_RX) · G10(IR_REC) · G11(SCL) · G12(SDA)** | 舵机/RGB/IR/NFC/触摸/电量计 | 器件细节见 [本体模组页](../../modules/m5stack-stackchan-body/README.md)，本页不重复 |
| 电源键 / RST 键 | AXP2101 电源管理 / EN | 开关机、复位、下载模式 | 见「特殊说明」 |

> **内部 I2C = G11(SCL) / G12(SDA)**：CoreS3 的板内器件与本体模组器件**共用同一条 I2C 总线**（经 M-Bus pin17/18 引到本体适配板），地址不冲突；完整地址总表见 [本体模组页「接口」](../../modules/m5stack-stackchan-body/README.md#接口)。

### 摄像头 GC0308 接线（CoreS3 板内）

> 据官方 CoreS3 页 PinMap 转录（快照 `raw/datasheets/m5stack-cores3-doc.txt`，2026-08-20）。注意：**XCLK 与 PWDN 未接任何 ESP32 GPIO**（官方标 `-1`），复位由 IO 扩展 AW9523B 驱动；SCCB 走 G11/G12 那条共享内部 I2C（地址 0x21）。

| GC0308 信号 | 含义 | CoreS3 GPIO |
|---|---|---|
| SIOC | SCCB 时钟 | **G11** |
| SIOD | SCCB 数据 | **G12** |
| XCLK | 外部主时钟 | **未接**（-1） |
| VSYNC | 帧同步 | **G46** |
| HREF | 行有效 | **G38** |
| PCLK | 像素时钟 | **G45** |
| D0 | 数据位0 | **G39** |
| D1 | 数据位1 | **G40** |
| D2 | 数据位2 | **G41** |
| D3 | 数据位3 | **G42** |
| D4 | 数据位4 | **G15** |
| D5 | 数据位5 | **G16** |
| D6 | 数据位6 | **G48** |
| D7 | 数据位7 | **G47** |
| RESET# | 复位 | 未接 GPIO，经 **AW9523B P1_0** |
| PWDN | 掉电 | **未接**（-1） |

> GC0308 与光距感 LTR-553ALS-WA 官方说明集成在同一条排线上。出场固件里语音 "What can you see right now?" 抓拍、App 的 Monitoring Camera 模式（云台转角度的监控摄像头）用的就是它。

## 引出引脚

> 表 B：对外扩展 = CoreS3 自带 **3 × HY2.0-4P（Grove）**。线序色标（黑/红/黄/白）为官方统一约定。组装后三个口的可达位置/朝向 **TODO: 待核实**（需实机确认）。

| 丝印 | 类型 | 黑 | 红 | 黄 | 白 | 功能 |
|---|---|---|---|---|---|---|
| PORT.A | HY2.0-4P | GND | 5V | G2（SDA） | G1（SCL） | I2C |
| PORT.B | HY2.0-4P | GND | 5V | G9（PB_OUT） | G8（PB_IN） | 通用 IO（ADC/DAC） |
| PORT.C | HY2.0-4P | GND | 5V | G17（PC_TX） | G18（PC_RX） | UART |

> PORT.A/B/C 的引脚与功能已对照官方 CoreS3 M-Bus 表核实（G2=PORT.A SDA、G1=PORT.A SCL、G8=PB_IN、G9=PB_OUT、G17=PC_TX、G18=PC_RX），见 `raw/datasheets/m5stack-cores3-doc.txt`。另有 LEGO 兼容安装孔。

## 引脚物理排布

> 纯文本转录整机各接口/部件的**已核实位置**（官方文字描述）；未注明的部件位置不臆测。

```text
正面视角：机器人立姿、屏幕朝自己（初始位 = 屏幕垂直于底座、居中于两脚之间）

        ┌───────────────────────────┐
        │    顶部三区触摸板 (Si12T)   │  ← 头顶，本体触摸板
   ┌───┴───────────────────────────┴───┐
电 │                                   │
源 │     2.0" 320×240 IPS 触摸屏        │  ← CoreS3（屏幕左上沿有一枚
键 │     （摄像头 GC0308 板内）          │     RGB LED 指示语音状态：
(主│                                   │     绿=聆听/蓝=说话/灭=空闲)
控 │  USB-C ①（主控侧，数据+供电）       │
左 └───────────────┬───────────────────┘
侧                 │ 舵机关节：X 轴 360° 连续（水平摇头）
                  │           Y 轴 90°（俯仰，限 5~85°）
           ┌──────┴──────────────────────┐
           │  底座（适配板/电源板/圈板/触摸板）│
           │  USB-C ②（底座侧，数据+供电，  │
           │          刷机推荐用此口）       │
           │  RST 键 + 指示 LED（microSD   │
           │  卡槽旁，底面）                │
           └─────────────────────────────┘
```

- 3 × Grove 口（PORT.A/B/C）物理位置在 CoreS3 上（见上一节表），组装后可达性待核实。
- microSD 卡槽：CoreS3 底部（RST 键旁）。
- 本体 4 块小板（适配/电源/圈/触摸）的相对位置与内部线序见 [本体模组页](../../modules/m5stack-stackchan-body/README.md#引脚物理排布)。

## 原理图

官方 5 份原理图已归档（本体 4 板 + CoreS3 主控）：

- [本体 · 适配板 SCH_Adapter](../../raw/schematics/m5stack-stackchan-sch-adapter.pdf)
- [本体 · 电源板 SCH_Power](../../raw/schematics/m5stack-stackchan-sch-power.pdf)
- [本体 · 圈板 SCH_Ring](../../raw/schematics/m5stack-stackchan-sch-ring.pdf)
- [本体 · 触摸板 SCH_Touch](../../raw/schematics/m5stack-stackchan-sch-touch.pdf)
- [主控 CoreS3 v1.0（7 页）](../../raw/schematics/m5stack-cores3-v1.0.pdf)
- 外形尺寸图：[Model_Size.pdf](../../raw/datasheets/m5stack-stackchan-model-size.pdf)；结构文件：[M5_Hardware/K151_StackChan](https://github.com/m5stack/M5_Hardware/tree/master/Products/K151_StackChan/Structures)

## 示例代码

- [`examples/body_i2c_scan/`](examples/body_i2c_scan/) — 在 G12(SDA)/G11(SCL) 上扫本体 I2C 总线，按预期地址表（INA226 0x41 / NFC 0x50 / 触摸 0x68 / PY32L020 0x6F…）核对本体器件是否在位，最小可用烧录验证。
- 舵机控制：串口总线舵机 SCS0009，TX=G6 / RX=G7，协议见 [SCS0009 数据手册](../../raw/datasheets/m5stack-stackchan-scs0009-servo.pdf)（官方 BSP：[StackChan-BSP](https://github.com/m5stack/StackChan-BSP)；TODO: 协议要点待整理成示例）。

## 特殊说明

- **开关机**：短按电源键开机（主控左侧、USB-C 旁）；**长按 6s 关机**；短按 RST 重启（底面 microSD 槽旁）。
- **下载模式**：接 USB-C（两个口都支持数据，**推荐底座口**，避免电机误动伤人），**按住 RST 3s** 至旁边指示 LED 变绿后松开（灯灭即进入下载模式）。
- **舵机安全**：电机通电受控时勿强行手掰；**Y 轴（俯仰）限 5~85°**，超限可能堵转永久损坏；X 轴无角度限制（360° 连续）。
- **Stackchan App 兼容性**：只连**原装 StackChan Core**（标准版 / 遥控套装内的 / 单卖的替换主控）；裸 CoreS3 / CoreS3-SE / CoreS3-Lite 刷 StackChan 用户固件后才可连。
- **小智配对体系互斥**：出厂固件经 StackChan World App 配置；刷任何其他小智（xiaozhi.me）固件前必须先解绑，反之亦然，否则配对异常。
- **网络**：仅 2.4GHz Wi-Fi，不支持 5GHz / 强制 Portal 认证的网络。
- **恢复出厂**：M5Burner 搜 StackChan（勾选 Only Official）烧录。
- 出厂 AI Agent 支持语音控制自身部件（音量/转头/灯色/拍照/电量）及 Home Assistant MCP 设备控制。

## 相关页面

- 🔗 内含模组：[M5Stack Stack-chan Body（机器人本体）](../../modules/m5stack-stackchan-body/README.md)——舵机/RGB/IR/NFC/触摸/电量计/PY32L020 的事实源
- 主控：CoreS3（官方页 <https://docs.m5stack.com/en/core/CoreS3>）——**本 wiki 独立页待建**（快照含完整 PinMap 已备好，摄像头接线已先行转录至本页）
- 遥控器相关：[M5StickC Plus2](../m5stack-stickc-plus2/README.md)（同系列，遥控器实际用的是旧款 StickC-Plus）、[Hat Mini JoyC](https://docs.m5stack.com/en/hat/MiniJoyC)
- 同芯片（ESP32-S3）的板子：[M5Stack StickS3](../m5stack-sticks3/README.md)、[M5Stack StopWatch](../m5stack-stopwatch/README.md)、[ESP32-S3 SuperMini](../esp32-s3-supermini/README.md)、[ESP32-S3-AI-Board](../esp32-s3-ai-board/README.md)、[Waveshare ESP32-S3-Matrix](../waveshare-esp32-s3-matrix/README.md)

## 参考来源

- `raw/datasheets/m5stack-stackchan-doc.txt` — 官方 StackChan 页面全文快照（2026-08-20）
- `raw/datasheets/m5stack-cores3-doc.txt` — 官方 CoreS3 页面全文快照（2026-08-20，PinMap/M-Bus/I2C 地址表，用于核实 Grove 口与内部总线）
- `raw/schematics/m5stack-stackchan-sch-{adapter,power,ring,touch}.pdf` + `raw/schematics/m5stack-cores3-v1.0.pdf` — 官方原理图
- `raw/assets/m5stack-stackchan-main.webp` — 官方整机照
- 官方固件开源仓库：<https://github.com/m5stack/StackChan>（设备固件 / 遥控器固件 / App / 服务端）· BSP：<https://github.com/m5stack/StackChan-BSP> · Arduino 教程：<https://docs.m5stack.com/en/arduino/stackchan/program> · UiFlow2 教程：<https://docs.m5stack.com/en/uiflow2/stackchan/program>
- TODO 待核实：组装后 Grove 口可达位置；CoreS3 内置电池与本体外 550mAh 电池的关系；SCS0009 串口协议要点（数据手册已归档未译读）
