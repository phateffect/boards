// M5StickC Plus2 — 板载红色 LED 闪烁 (GPIO10)
// 极性：M5StickC 红色 LED 通常高电平点亮（待核实；若不亮把 LED_LEVEL 取反）
#include <Arduino.h>

#ifndef LED_PIN
#define LED_PIN 10
#endif
static const int LED_LEVEL = HIGH;   // 点亮电平

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("[stickc-plus2] blink onboard red LED @ GPIO10");
}

void loop() {
    digitalWrite(LED_PIN, LED_LEVEL);
    delay(500);
    digitalWrite(LED_PIN, !LED_LEVEL);
    delay(500);
}
