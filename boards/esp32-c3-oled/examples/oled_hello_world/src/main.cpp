/*
 * ESP32-C3 OLED 驱动示例（0.42" 72x40 SSD1306，I2C）
 * 屏体事实见 boards/esp32-c3-oled/README.md 与 raw/c3-oled-screen.pdf：
 *   分辨率 72x40（非 128x64），SSD1306，I2C 地址 0x3C，SDA=GPIO5 / SCL=GPIO6。
 * 显示 ESP32-C3 + 计数器 + 堆内存监控。
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  72        // 0.42" 屏实际像素（规格书 N042-7240TSWEG01-H16，1/40 duty）
#define SCREEN_HEIGHT 40
#define OLED_ADDR     0x3C      // SA0=0 -> 7 位地址 0x3C（写 0x78）

#define SDA_PIN 5
#define SCL_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED 初始化失败！复查 SDA=5 / SCL=6 与地址 0x3C");
    while (1);
  }
  Serial.println("OLED 就绪 (72x40)");
  display.clearDisplay();
  display.display();
}

void loop() {
  static int counter = 0;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("ESP32-C3");

  display.setCursor(0, 12);
  display.print("cnt:");
  display.println(counter);

  display.setCursor(0, 22);
  display.print("heap:");
  display.print(esp_get_free_heap_size() / 1024);
  display.println("KB");

  display.setCursor(0, 32);
  display.println("0.42\" OLED");

  display.display();
  counter++;
  delay(1000);
}
