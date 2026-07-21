// M5StickC Plus2 — 板载红色 LED 闪烁
// 注意：Plus2 的红色 LED 在 GPIO19，且与 IR 发射管【共用同一引脚】。
//       toggle G19 会同时驱动 IR（肉眼不可见），只会看到红 LED 亮灭。
// （Plus 的红 LED 是 G10，Plus2 已改——别照搬老代码。）
#include <Arduino.h>

#ifndef LED_PIN
#define LED_PIN 19
#endif

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("[stickc-plus2] blink red LED @ GPIO19 (shared with IR)");
}

void loop() {
    digitalWrite(LED_PIN, HIGH);   // 红 LED 亮（IR 也被驱动，不可见）
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
}
