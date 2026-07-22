/*
 * ESP32-C3 OLED 简化版驱动
 * 更小的内存占用，适合复杂项目
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// I2C引脚配置
#define SDA_PIN 5
#define SCL_PIN 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // 初始化I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // 初始化OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED初始化失败！");
    while(1);
  }
  
  Serial.println("OLED就绪");
  display.clearDisplay();
  
  // 显示欢迎信息
  showHelloWorld();
}

void loop() {
  // 显示时间和滚动效果
  displayTime();
  delay(1000);
}

// 显示Hello World
void showHelloWorld() {
  display.clearDisplay();
  
  // 顶部标题
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("=== ESP32-C3 ===");
  
  // 主标题
  display.setTextSize(3);
  display.setCursor(15, 15);
  display.println("Hello");
  
  display.setTextSize(3);
  display.setCursor(25, 40);
  display.println("World");
  
  // 底部信息
  display.setTextSize(1);
  display.setCursor(25, 58);
  display.println("0.96\" OLED");
  
  display.display();
}

// 显示时间
void displayTime() {
  static int counter = 0;
  
  display.clearDisplay();
  
  // 边框    右边距：28  上边距：24    右边距128-72     下边距：64-40
  display.drawRect(28, 24, 72, 40, WHITE);
  
  // 显示信息
  display.setCursor(30, 26);
  display.print("Counter:");
  display.println(counter);
  
  display.setCursor(30, 34);
  display.print("Memory: ");
  display.print(esp_get_free_heap_size() / 1024);
  display.println(" KB");
  
  // 底部显示Hello World
  display.setTextSize(1);
  display.setCursor(30, 42);
  display.println("Hello World");

  // 底部显示Come on ！ Come on ！
  display.setTextSize(1);
  display.setCursor(30, 50);
  display.println("Come on !");
  
  display.display();
  
  counter++;
}
