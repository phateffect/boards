#include <Arduino.h>

// 模组驱动示例模板。复制到 modules/<slug>/examples/<name>/ 后改。
// 把模组相关引脚集中在 platformio.ini 的 build_flags 里。

#ifndef SDA_PIN
#define SDA_PIN -1
#endif
#ifndef SCL_PIN
#define SCL_PIN -1
#endif

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.printf("[template] module driver on SDA=%d SCL=%d\n", SDA_PIN, SCL_PIN);
}

void loop() {
    // TODO: 读传感器 / 点亮器件
    delay(1000);
}
