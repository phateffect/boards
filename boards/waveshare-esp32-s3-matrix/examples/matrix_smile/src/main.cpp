// Waveshare ESP32-S3-Matrix — 8x8 WS2812 矩阵演示 (GPIO14)
// 依次点亮一颗"游走"的灯珠 + 整屏颜色循环，验证矩阵接线。
// 板子事实（见 wiki 板子页「特殊说明」/ raw 踩坑流水）：
//   字节序 = RGB → 用 NEO_RGB（非默认 NEO_GRB，否则红绿互换）
//   像素映射 = 行主序、非蛇形、idx0=左上角（本例 i=0..63 即按此顺序游走）
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

Adafruit_NeoPixel strip(N, MATRIX_PIN, NEO_RGB + NEO_KHZ800);   // 实测字节序为 RGB（非 GRB）

void allOff() { strip.clear(); strip.show(); }

void setup() {
    Serial.begin(115200);
    strip.begin();
    strip.setBrightness(16);   // 保持低亮度（官方提示高亮度过热）；硬件下限是 1，再暗需 app 抖动(会频闪)
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
