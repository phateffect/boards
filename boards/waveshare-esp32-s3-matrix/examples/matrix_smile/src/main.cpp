// Waveshare ESP32-S3-Matrix — 8x8 WS2812 矩阵演示 (GPIO14)
// 依次点亮一颗"游走"的灯珠 + 整屏颜色循环，验证矩阵接线。
#include <Adafruit_NeoPixel.h>

#ifndef MATRIX_PIN
#define MATRIX_PIN 14
#endif
#ifndef MATRIX_W
#define MATRIX_W 8
#endif
#ifndef MATRIX_H
#define MATRIX_H 8
#endif
#define N (MATRIX_W * MATRIX_H)

Adafruit_NeoPixel strip(N, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

void allOff() { strip.clear(); strip.show(); }

void setup() {
    Serial.begin(115200);
    strip.begin();
    strip.setBrightness(20);   // 别太高，官方提示高亮度会过热
    strip.clear();
    strip.show();
    Serial.println("[esp32-s3-matrix] 8x8 matrix @ GPIO14");
}

void loop() {
    // 1) 游走亮点
    for (int i = 0; i < N; i++) {
        strip.clear();
        strip.setPixelColor(i, strip.Color(0, 0, 255));
        strip.show();
        delay(40);
    }
    // 2) 整屏红/绿/蓝
    uint32_t cols[3] = { strip.Color(40, 0, 0), strip.Color(0, 40, 0), strip.Color(0, 0, 40) };
    for (auto c : cols) {
        strip.fill(c);
        strip.show();
        delay(400);
    }
    allOff();
    delay(300);
}
