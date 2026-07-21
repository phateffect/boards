// Waveshare ESP32-S3-Matrix — QMI8658 IMU 最小探测示例
// 原理图核实：SDA=GPIO11, SCL=GPIO12, 7 位地址 0x6A
// 本例只做 I2C 扫描 + 读 WHO_AM_I(寄存器 0x00，应返回 0x05)，验证接线/地址。
// 完整加速度/陀螺仪读取请用 Waveshare SensorLib（lib_deps: lewisxhe/SensorLib）。
#include <Wire.h>
#include <Arduino.h>

#ifndef I2C_SDA
#define I2C_SDA 11
#endif
#ifndef I2C_SCL
#define I2C_SCL 12
#endif
#define QMI_ADDR   0x6A
#define QMI_WHOAMI 0x00   // 期望读到 0x05

static uint8_t readReg(uint8_t addr, uint8_t reg) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom((int)addr, 1);
    return Wire.available() ? Wire.read() : 0xFF;
}

void setup() {
    Serial.begin(115200);
    delay(200);
    Wire.begin(I2C_SDA, I2C_SCL);
    Serial.printf("[esp32-s3-matrix] I2C on SDA=%d SCL=%d\n", I2C_SDA, I2C_SCL);

    Serial.println("I2C 扫描:");
    for (uint8_t a = 1; a < 0x7F; a++) {
        Wire.beginTransmission(a);
        if (Wire.endTransmission() == 0) Serial.printf("  发现设备 @0x%02X\n", a);
    }

    uint8_t who = readReg(QMI_ADDR, QMI_WHOAMI);
    Serial.printf("QMI8658 @0x%02X  WHO_AM_I(0x00)=0x%02X (期望 0x05)\n", QMI_ADDR, who);
    Serial.println(who == 0x05 ? "✅ IMU 接线/地址核实通过" : "❌ 未识别，复查 11/12 与 0x6A");
}

void loop() {}
