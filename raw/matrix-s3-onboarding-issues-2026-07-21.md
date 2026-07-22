# Waveshare ESP32-S3-Matrix 点灯 / 配网 Onboarding 踩坑记录

> 把一块 **Waveshare ESP32-S3-Matrix**（ESP32-S3，4MB flash，8×8 WS2812）从零跑起来（PlatformIO + Arduino），再到写一个 WiFi 配网小程序（开 AP 门户选网 + 矩阵状态指示 + BOOT 长按清配置）过程中遇到的坑。
>
> 每条都给「现象 → 根因 → 解法」，并附可直接复用的配置/代码。给后来人（包括未来的我）省时间。
>
> - 日期：2026-07-21
> - 工具链：PlatformIO `espressif32@6.7.0`（Arduino core 2.0.16）/ macOS
> - 相关板子 wiki 页：[`../../boards/waveshare-esp32-s3-matrix/README.md`](../../boards/waveshare-esp32-s3-matrix/README.md)
> - 项目（固件本体）：`matrix-gateway/`

---

## 速查（坑一览）

| # | 现象 | 根因 | 解法 |
|---|---|---|---|
| 0 | `pio device list` 看到的 USB 串口不是板子 | 数据线不对（枚举成别的设备） | 换**能传数据**的 USB-C 线 |
| 1 | 上电后反复重启（boot loop） | 镜像头写 8MB，实际只有 4MB flash | 自定义 board JSON（4MB + `default.csv`） |
| 2 | `pio run -t upload` 烧不进去 | USB-Serial-JTAG 上 esptool 的 stub 不稳 | 用 `esptool --no-stub` 烧录 |
| 3 | 串口读不到 / 读一下就掉线 | pyserial 的 open 触发 USB-Serial-JTAG 自动复位；且 `Serial=UART0` 在 app 起来后 ROM 桥就停了 | raw `os.open()` 读串口 + `ARDUINO_USB_CDC_ON_BOOT=1` |
| 4 | 红绿反了（红显示成绿，青↔品红互换，橙→黄绿） | WS2812 字节序是 **RGB** 不是 GRB | Adafruit NeoPixel 用 `NEO_RGB` |
| 5 | 扩散动画在奇数行错位 | 误以为是蛇形走线 | 标准行主序、**非蛇形**、`idx0=左上角` |
| 6 | 太亮（即便 1% 占空比） | `setBrightness` 没压到最低 | `setBrightness(1)`（硬件最低档） |

---

## 板子最终确认到的事实（标定结论）

- **USB-C = ESP32-S3 原生 USB-Serial-JTAG**：枚举为 `303A:1001` "USB JTAG/serial debug unit"，**没有 USB-UART 桥**（CH340/CP2102 之类）。
- **Flash 4MB，无 PSRAM。**
- 8×8 WS2812 矩阵 DIN = **GPIO14**（Arduino core 的 `PIN_NEOPIXEL`）；矩阵 DOUT 引出在 GPIO37 旁焊盘。
- **WS2812 字节序 = RGB**（不是最常见的 GRB；见坑 4）。
- **像素映射 = 行主序、非蛇形**：`idx0 = 左上角`，行向下、列向右（见坑 5）。
- 官方提示高亮度会过热；实际把亮度压到 `setBrightness(1)` 才舒服（见坑 6）。

> 以上这几条（字节序 RGB、像素映射、USB-Serial-JTAG 烧录/监视的坑）建议回头 ingest 进板子 wiki 页 `boards/waveshare-esp32-s3-matrix/README.md`，本文件只是原始踩坑流水。

---

## 坑 0：USB 线不对，枚举成别的设备

**现象**：插上板子，`pio device list` 看到一个串口，但描述是 `LDR2001`、VID:PID=`2D79:0003`，esptool 连不上（`Failed to connect to ESP32-S3: No serial data received`）。

**根因**：那根 USB-C 线是**充电线 / 不全导通的线**，枚举出来的设备根本不是 ESP32-S3。

**解法**：换一根**能传数据**的 USB-C 线。换线后正确枚举为：
```
/dev/cu.usbmodem1101
Hardware ID: USB VID:PID=303A:1001
Description: USB JTAG/serial debug unit
```
> 经验：ESP32-S3 原生 USB 应当是 **VID `303A`**（Espressif）。看到 `2D79` 之类就先怀疑线/设备。

---

## 坑 1：boot loop —— 镜像头 8MB vs 实际 4MB flash

**现象**：烧录成功，但串口里反复重启，关键报错：
```
E (81) spi_flash: Detected size(4096k) smaller than the size in the binary image header(8192k). Probe failed.
assert failed: do_core_init startup.c:328 (flash_ret == ESP_OK)
rst:0xc (RTC_SW_CPU_RST) ... Rebooting...
```

**根因**：PlatformIO 的 `esp32-s3-devkitc-1` 其实是 **-N8（8MB flash）变体**，其 board JSON 里 `upload.flash_size = 8MB` 且分区表是 `default_8MB.csv`。elf2image 据此把**镜像头写成 8MB**，但本板只有 4MB → 二级 bootloader 里 `spi_flash` 探测失败 → assert → 循环重启。

**关键误区**：`board_build.flash_size = 4MB` **没用**。espressif32 平台的 elf2image 读的是 `upload.flash_size`（board JSON），不是 `build.flash_size`（`board_build.flash_size` 映射到的那个）。验证：`pio run -v` 里能看到
```
elf2image --flash_size 8MB -o bootloader.bin ...
elf2image --flash_size 8MB ... -o firmware.bin ...
```
即便 `board_build.flash_size=4MB`，这两条仍是 8MB。

**解法**：写一个**自定义 board JSON**（项目里放 `custom_boards/waveshare_esp32_s3_matrix.json`），把 `upload.flash_size` 和分区表都改 4MB：

```json
{
  "build": {
    "arduino": { "ldscript": "esp32s3_out.ld", "partitions": "default.csv" },
    "core": "esp32",
    "extra_flags": ["-DARDUINO_ESP32S3_DEV", "-DARDUINO_USB_MODE=1",
                    "-DARDUINO_RUNNING_CORE=1", "-DARDUINO_EVENT_RUNNING_CORE=1"],
    "f_cpu": "240000000L", "f_flash": "80000000L", "flash_mode": "qio",
    "hwids": [["0x303A", "0x1001"]], "mcu": "esp32s3", "variant": "esp32s3"
  },
  "frameworks": ["arduino", "espidf"],
  "name": "Waveshare ESP32-S3-Matrix (4MB QD, No PSRAM)",
  "upload": {
    "flash_size": "4MB", "maximum_ram_size": 327680,
    "maximum_size": 4194304, "require_upload_port": true, "speed": 460800
  },
  "url": "https://docs.waveshare.com/ESP32-S3-Matrix", "vendor": "Waveshare"
}
```

`platformio.ini`：
```ini
[platformio]
default_envs = matrix
boards_dir = custom_boards        ; 自定义 board（注意：别和 boards/ 子模块目录撞名）

[env:matrix]
platform   = espressif32@6.7.0
board      = waveshare_esp32_s3_matrix   ; 用上面的 4MB 自定义 board
framework  = arduino
monitor_speed = 115200
upload_speed  = 115200
```
> 注意：PlatformIO 默认从项目根的 `boards/` 读自定义 board；如果 `boards/` 已被你的 git submodule 占用，用 `boards_dir = custom_boards` 改到别的目录。

改完再 `pio run -v`，elf2image 应变成 `--flash_size 4MB`，启动不再 assert。

---

## 坑 2：USB-Serial-JTAG 上 esptool 的 stub 不稳，烧不进去

**现象**：`pio run -t upload`（esptool 默认用软 stub）时好时坏，典型失败：
```
Uploading stub... Running stub... Stub running...
Changing baud rate to 460800
A fatal error occurred: No serial data received.
```
或
```
A fatal error occurred: Unable to verify flash chip connection (No serial data received.).
```
921600 / 460800 / 115200 都试过，stub 总在「切波特率 / 读 flash ID」这一步掉。

**根因**：ESP32-S3 的 USB-Serial-JTAG 接口上，esptool 下载的软 stub 重新配置 UART 时容易把链路搞挂（stub 能上传、但后续握手失败）。先前偶尔能成功纯属运气。

**解法**：用 **ROM 模式 `--no-stub`** 烧录（不下载 stub、不切波特率，最稳）。命令模板（偏移量是 Arduino-ESP32 默认布局）：
```bash
esptool.py --chip esp32s3 --port /dev/cu.usbmodem1101 --baud 115200 \
  --before default_reset --after hard_reset --no-stub \
  write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB \
  0x0     .pio/build/matrix/bootloader.bin \
  0x8000  .pio/build/matrix/partitions.bin \
  0xe000  <framework-arduinoespressif32>/tools/partitions/boot_app0.bin \
  0x10000 .pio/build/matrix/firmware.bin
```
项目里把它封装成了一个 mise task：`mise run flash -- --port /dev/cu.usbmodem1101`（先 `pio run`，再上面的 `--no-stub`）。**别再用 `pio run -t upload`。**

---

## 坑 3：串口读不到 / 一读就掉线

这一坑其实有两个独立的点，都得处理。

### 3a. pyserial 的 `Serial()` 会触发 USB-Serial-JTAG 自动复位

**现象**：用 pyserial 打开 `/dev/cu.usbmodem1101`，串口里能看到 ROM 启动日志，然后报
```
Error: failed to read serial port /dev/cu.usbmodem1101: read failed: [Errno 6] Device not configured
```
每次开端口都能看到一次 `rst:0x15 (USB_UART_CHIP_RESET)`。

**根因**：pyserial 的 `Serial()` 构造里会 `tcsetattr()` 重配 termios，顺带拉一下 DTR/RTS；ESP32-S3 的 USB-Serial-JTAG 把这个电平变化当成复位请求 → 芯片复位 → CDC 重新枚举 → 旧的 fd 失效（macOS 上就是 ENXIO "Device not configured"）。

**解法**：读串口时用 **raw `os.open()`，不走 termios**（不拉 DTR/RTS，就不复位）：
```python
fd = os.open(port, os.O_RDONLY | os.O_NONBLOCK)   # 不触发 tcsetattr → 不复位
end = time.monotonic() + duration
while time.monotonic() < end:
    r, _, _ = select.select([fd], [], [], 0.2)
    if r:
        chunk = os.read(fd, 1024)
        ...
```
> `pio device monitor` 也能用（它内部会重连），但它需要**交互式 TTY**（走 miniterm/termios），在无 TTY 的脚本环境会报 `termios.error: (19, 'Operation not supported by device')`。所以脚本/CI 里还是得 raw open。

### 3b. `Serial` 必须走 HWCDC（`ARDUINO_USB_CDC_ON_BOOT=1`），否则 app 打印到 USB 看不到

**现象**：一开始为了"稳定"把 `ARDUINO_USB_CDC_ON_BOOT` 关掉（让 `Serial=UART0`），结果 app 的 `Serial.println` 在 USB 上**完全看不到**（只能看到 ROM 启动那几行）。

**根因**：本板唯一的串口通路就是 USB-Serial-JTAG。ROM 在下载/启动阶段会把 UART0 桥接到 USB；但 **app 一旦接管、ROM 桥就停了**，而 Arduino core 默认 `Serial=UART0` 并不会去驱动 USB-Serial-JTAG → app 打印上不了 USB。

**解法**：开 `ARDUINO_USB_CDC_ON_BOOT=1`，让 `Serial = HWCDC`（core 主动驱动 USB-Serial-JTAG）：
```ini
build_flags =
  -DARDUINO_USB_CDC_ON_BOOT=1   ; 本板唯一串口=USB-Serial-JTAG，app 必须驱动它才有输出
```
配合 3a 的 raw-open 读法，app 打印就能稳定读到了。

---

## 坑 4：WS2812 颜色 R/G 反了（字节序是 RGB 不是 GRB）

**现象**：扩散动画颜色不对；用纯原色标定时发现「红显示成绿、绿显示成红、蓝正常、青↔品红互换、橙(255,128,0)→黄绿、黄/白不变」。

**根因**：Adafruit NeoPixel 默认 `NEO_GRB`，但这块板的 WS2812 字节序是 **RGB**。在 `NEO_GRB` 下，库按 G,R,B 顺序发字节，LED 按 R,G,B 读 → R、G 正好互换。

**怎么诊断的**：每"数据行"（每 8 颗）涂一种纯原色（红/绿/蓝/黄/青/品红/橙/白），看实际显示。红绿互换是字节序错的典型特征（蓝不变、R=G 的黄/白不变、青↔品红互换）。

**解法**：把 NeoPixel 声明改成 `NEO_RGB`：
```cpp
Adafruit_NeoPixel strip(N, PIN, NEO_RGB + NEO_KHZ800);   // 实测字节序为 RGB
```

> 教训：低亮度下用 HSV 渐变去猜布局/颜色会被误导（颜色分不清）。标定布局/字节序时，直接用**纯 RGB 原色**最清楚。

---

## 坑 5：像素布局误判成蛇形

**现象**：扩散动画（从一个原点按切比雪夫距离一圈圈铺开）在奇数行看起来错位、不像规整的方形波纹。

**根因**：一开始假设 8×8 矩阵是"行主序 + 蛇形"（偶数行 L→R、奇数行 R→L），给 `idxToXY` 加了奇数行翻转。实际本板是**标准行主序、非蛇形**。

**怎么标定的**（和坑 4 同一次纯原色行测试）：每"数据行"涂一种纯色，从上到下读颜色顺序 → 直接得到「数据第几行 = 视觉第几行」。再结合一次四角点亮（idx0/7/56/63），定死：

> **行主序、非蛇形、`idx0 = 左上角`，行向下、列向右。**

**解法**：`idxToXY` 就是最朴素的形式：
```cpp
static void idxToXY(uint16_t i, uint8_t& x, uint8_t& y) {
    y = i / MATRIX_W;   // 行：上→下
    x = i % MATRIX_W;   // 列：左→右
}
```

> 补充：坑 4（红绿互换）一度让基于颜色的标定结果互相矛盾（四角测试、三行测试对不上），把颜色字节序修对之后，所有标定结果才一次性自洽。所以**先修字节序、再标布局**。

---

## 坑 6：亮度压到最低

**现象**：逐次调暗（10% → 5% → 3% → 2% → 1% 占空比）用户仍嫌亮；要"无论何种模式都用最低亮度"。

**根因**：WS2812 的亮度由每通道 PWM 占空比决定，Adafruit 的 `setBrightness(b)` 在 `show()` 时把每个通道乘以 `b/255`。要做到全模式统一最低，就得**全局设一次**且任何模式都不改它。

**解法**：全局定义 + `initBoard()` 里设一次：
```cpp
// 所有模式（配网动画 / 白色重置闪烁 / …）统一硬件最低亮度。
constexpr uint8_t LIT_BRIGHTNESS = 1;   // 1 是「亮」的最低档；0 = 灭
void initBoard() {
    strip.begin();
    strip.setBrightness(LIT_BRIGHTNESS);
    strip.clear(); strip.show();
}
```
> `setBrightness(1)` 是硬件下限。再暗只能在 app 层做**时间抖动**（整个矩阵亮 X ms / 灭 Y ms，等效再降占空比），代价是轻微频闪。

---

## 附：可复用的项目骨架

```
matrix-gateway/
├── platformio.ini                 # env:matrix / 自定义 4MB board / CDC_ON_BOOT
├── custom_boards/
│   └── waveshare_esp32_s3_matrix.json   # 4MB flash + default.csv（解坑 1）
├── src/
│   ├── main.cpp                   # 状态机：INIT(AP 门户) / STA_CONNECTING / CONNECTED
│   ├── provision.{h,cpp}          # NVS 存凭据 + AP+DNS 通配门户 + STA 连接
│   └── devices/
│       ├── board.{h,cpp}          # 矩阵常量 + setBrightness(1) + NEO_RGB + matrixColorHSV/RGB
│       └── button.{h,cpp}         # BOOT(GPIO0) 长按检测
├── .mise/tasks/
│   ├── flash                      # pio run + esptool --no-stub（解坑 2）
│   ├── serial                     # raw os.open() 读串口（解坑 3a）
│   └── lint                       # clang-format
└── boards/                        # git submodule：板子知识库 wiki
```

关键三件事一句话：
1. **4MB flash** → 自定义 board JSON（别指望 `board_build.flash_size`）。
2. **烧录用 `--no-stub`，串口用 raw open + `CDC_ON_BOOT=1`。**
3. **WS2812 是 `NEO_RGB`，像素行主序非蛇形 `idx0=左上角`，亮度 `setBrightness(1)`。**
