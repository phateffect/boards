#include <Arduino.h>

#ifndef LED_PIN
#define LED_PIN 2
#endif

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("[template] hello from Boards Wiki example");
}

void loop() {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
}
