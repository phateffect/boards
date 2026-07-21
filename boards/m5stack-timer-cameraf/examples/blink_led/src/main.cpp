// M5Stack Timer Camera F — 板载状态 LED 闪烁 (GPIO2)
#include <Arduino.h>

#ifndef LED_PIN
#define LED_PIN 2
#endif

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("[timer-cameraf] blink status LED @ GPIO2");
}

void loop() {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
}
