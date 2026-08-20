// M5Stack Stack-chan (K151) — 机器人本体 I2C 总线扫描
//
// 事实来源（官方 StackChan 文档 PinMap / I2C Devices 表，2026-08-20 快照）：
//   本体 I2C 挂在 ESP32-S3 的 G12(SDA) / G11(SCL)，与 CoreS3 板内器件共用一条总线。
//   本体器件：INA226 0x41 / ST25R3916 0x50 / Si12T 0x68 / PY32L020 0x6F(默认) 或 0x71。
//   CoreS3 板内器件（官方 CoreS3 页 I2C 地址表）：
//     AXP2101 0x34 / AW88298 0x36 / FT6336U 0x38 / ES7210 0x40 / BM8563 0x51 /
//     AW9523 0x58 / BMI270 0x69 / BMM150 0x10 / GC0308 0x21 / LTR553 0x23
//
// 用途：最小烧录验证 + 核对本体器件在位。部分 CoreS3 板内器件可能因供电
// 状态不应答（如摄像头/光距感），扫描结果缺它们不代表故障。

#include <Arduino.h>
#include <Wire.h>

#ifndef I2C_SDA
#define I2C_SDA 12
#endif
#ifndef I2C_SCL
#define I2C_SCL 11
#endif

struct DeviceInfo {
    uint8_t addr;
    const char* name;
    const char* owner;   // body = 机器人本体 / core = CoreS3 板内
};

static const DeviceInfo kKnown[] = {
    {0x10, "BMM150 地磁",       "core"},
    {0x21, "GC0308 摄像头",     "core"},
    {0x23, "LTR-553ALS 光距感", "core"},
    {0x34, "AXP2101 PMIC",      "core"},
    {0x36, "AW88298 功放",      "core"},
    {0x38, "FT6336U 触摸",      "core"},
    {0x40, "ES7210 麦克风编解码", "core"},
    {0x41, "INA226 电量计",     "body"},
    {0x50, "ST25R3916 NFC",     "body"},
    {0x51, "BM8563 RTC",        "core"},
    {0x58, "AW9523 IO 扩展",    "core"},
    {0x68, "Si12T 顶部触摸",    "body"},
    {0x6F, "PY32L020 IO 扩展(默认)", "body"},
    {0x71, "PY32L020 IO 扩展(ADD_SEL=高)", "body"},
    {0x69, "BMI270 IMU",        "core"},
};

static const char* lookup(uint8_t addr) {
    for (const auto& d : kKnown) {
        if (d.addr == addr) return d.name;
    }
    return nullptr;
}

void setup() {
    Serial.begin(115200);
    delay(300);
    Serial.println();
    Serial.println("[stackchan] body I2C scan on G12(SDA) / G11(SCL)");
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(100000);
}

void loop() {
    Serial.println("--- scan start ---");
    int bodyFound = 0;
    for (uint8_t a = 1; a < 127; a++) {
        Wire.beginTransmission(a);
        if (Wire.endTransmission() == 0) {
            const char* name = lookup(a);
            const char* owner = nullptr;
            for (const auto& d : kKnown) {
                if (d.addr == a) { owner = d.owner; break; }
            }
            if (name && owner) {
                Serial.printf("  0x%02X  %-28s [%s]\r\n", a, name, owner);
                if (strcmp(owner, "body") == 0) bodyFound++;
            } else if (name) {
                Serial.printf("  0x%02X  %s\r\n", a, name);
            } else {
                Serial.printf("  0x%02X  (未知器件)\r\n", a);
            }
            delay(5);
        }
    }
    Serial.printf("--- scan end: %d 个本体器件应答（预期 4：0x41/0x50/0x68/0x6F）---\r\n", bodyFound);
    delay(5000);
}
