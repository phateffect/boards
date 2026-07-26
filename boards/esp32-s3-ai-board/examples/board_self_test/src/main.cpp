#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>
#include <SPI.h>
#include <driver/i2s.h>

#ifndef ENABLE_LCD_TEST
#define ENABLE_LCD_TEST 1
#endif
#ifndef ENABLE_RGB_TEST
#define ENABLE_RGB_TEST 1
#endif
#ifndef ENABLE_MIC_TEST
#define ENABLE_MIC_TEST 1
#endif
#ifndef ENABLE_SPEAKER_TEST
#define ENABLE_SPEAKER_TEST 1
#endif

namespace pins {
constexpr gpio_num_t kBoot = GPIO_NUM_0;
constexpr gpio_num_t kVolumeDown = GPIO_NUM_39;
constexpr gpio_num_t kVolumeUp = GPIO_NUM_40;
constexpr int kRgb = 48;

constexpr int kLcdBacklight = 3;
constexpr int kLcdMosi = 10;
constexpr int kLcdDc = 11;
constexpr int kLcdClock = 12;
constexpr int kLcdCs = 13;
constexpr int kLcdReset = 14;

constexpr gpio_num_t kMicWs = GPIO_NUM_4;
constexpr gpio_num_t kMicClock = GPIO_NUM_5;
constexpr gpio_num_t kMicData = GPIO_NUM_6;

constexpr gpio_num_t kSpeakerData = GPIO_NUM_7;
constexpr gpio_num_t kSpeakerClock = GPIO_NUM_15;
constexpr gpio_num_t kSpeakerLrClock = GPIO_NUM_16;
}  // namespace pins

SPIClass lcdSpi(FSPI);
Adafruit_ST7789 lcd(&lcdSpi, pins::kLcdCs, pins::kLcdDc, pins::kLcdReset);
Adafruit_NeoPixel rgb(1, pins::kRgb, NEO_GRB + NEO_KHZ800);

void testLcd() {
#if ENABLE_LCD_TEST
  Serial.println("[LCD] ST7789 170x320");
  pinMode(pins::kLcdBacklight, OUTPUT);
  digitalWrite(pins::kLcdBacklight, HIGH);
  lcdSpi.begin(pins::kLcdClock, -1, pins::kLcdMosi, pins::kLcdCs);
  lcd.init(170, 320);
  lcd.setRotation(3);
  lcd.fillScreen(ST77XX_BLACK);
  lcd.setCursor(12, 18);
  lcd.setTextColor(ST77XX_WHITE);
  lcd.setTextSize(2);
  lcd.println("ESP32-S3 AI Board");
  lcd.setTextColor(ST77XX_GREEN);
  lcd.println("LCD OK");
#endif
}

void testRgb() {
#if ENABLE_RGB_TEST
  Serial.println("[RGB] GPIO48");
  rgb.begin();
  rgb.setBrightness(24);
  for (uint32_t color :
       {rgb.Color(255, 0, 0), rgb.Color(0, 255, 0), rgb.Color(0, 0, 255)}) {
    rgb.setPixelColor(0, color);
    rgb.show();
    delay(250);
  }
  rgb.clear();
  rgb.show();
#endif
}

void configureButtons() {
  pinMode(pins::kBoot, INPUT_PULLUP);
  pinMode(pins::kVolumeDown, INPUT_PULLUP);
  pinMode(pins::kVolumeUp, INPUT_PULLUP);
}

void sampleMicrophone() {
#if ENABLE_MIC_TEST
  Serial.println("[MIC] sampling I2S level for 1 second");
  const i2s_config_t config = {
      .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      // The schematic ties the microphone L/R selector high but does not name
      // the microphone IC. Capture both slots so the test does not guess which
      // slot that particular device uses.
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 4,
      .dma_buf_len = 256,
      .use_apll = false,
      .tx_desc_auto_clear = false,
      .fixed_mclk = 0,
  };
  const i2s_pin_config_t pinsConfig = {
      .mck_io_num = I2S_PIN_NO_CHANGE,
      .bck_io_num = pins::kMicClock,
      .ws_io_num = pins::kMicWs,
      .data_out_num = I2S_PIN_NO_CHANGE,
      .data_in_num = pins::kMicData,
  };
  if (i2s_driver_install(I2S_NUM_0, &config, 0, nullptr) != ESP_OK ||
      i2s_set_pin(I2S_NUM_0, &pinsConfig) != ESP_OK) {
    Serial.println("[MIC] init failed");
    i2s_driver_uninstall(I2S_NUM_0);
    return;
  }

  int32_t samples[128]{};
  size_t bytesRead = 0;
  int32_t peak = 0;
  const uint32_t deadline = millis() + 1000;
  while (millis() < deadline) {
    if (i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead,
                 pdMS_TO_TICKS(100)) == ESP_OK) {
      for (size_t i = 0; i < bytesRead / sizeof(samples[0]); ++i) {
        const int32_t magnitude = abs(samples[i] >> 14);
        if (magnitude > peak) peak = magnitude;
      }
    }
  }
  Serial.printf("[MIC] peak=%ld (relative value)\n", static_cast<long>(peak));
  i2s_driver_uninstall(I2S_NUM_0);
#endif
}

void testSpeaker() {
#if ENABLE_SPEAKER_TEST
  Serial.println("[SPK] 440 Hz test tone");
  const i2s_config_t config = {
      .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 4,
      .dma_buf_len = 256,
      .use_apll = false,
      .tx_desc_auto_clear = true,
      .fixed_mclk = 0,
  };
  const i2s_pin_config_t pinsConfig = {
      .mck_io_num = I2S_PIN_NO_CHANGE,
      .bck_io_num = pins::kSpeakerClock,
      .ws_io_num = pins::kSpeakerLrClock,
      .data_out_num = pins::kSpeakerData,
      .data_in_num = I2S_PIN_NO_CHANGE,
  };
  if (i2s_driver_install(I2S_NUM_0, &config, 0, nullptr) != ESP_OK ||
      i2s_set_pin(I2S_NUM_0, &pinsConfig) != ESP_OK) {
    Serial.println("[SPK] init failed");
    i2s_driver_uninstall(I2S_NUM_0);
    return;
  }

  int16_t stereo[256]{};
  constexpr int kPeriod = 16000 / 440;
  for (int frame = 0; frame < 80; ++frame) {
    for (size_t i = 0; i < 128; ++i) {
      const int16_t sample = ((frame * 128 + i) % kPeriod) < (kPeriod / 2)
                                 ? 1200
                                 : -1200;
      stereo[i * 2] = sample;
      stereo[i * 2 + 1] = sample;
    }
    size_t bytesWritten = 0;
    i2s_write(I2S_NUM_0, stereo, sizeof(stereo), &bytesWritten,
              portMAX_DELAY);
  }
  i2s_zero_dma_buffer(I2S_NUM_0);
  i2s_driver_uninstall(I2S_NUM_0);
#endif
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\nESP32-S3-AI-Board staged self-test");
  configureButtons();
  testLcd();
  testRgb();
  sampleMicrophone();
  testSpeaker();
  Serial.println("[DONE] Press BOOT / VOL- / VOL+ to verify buttons.");
}

void loop() {
  static uint8_t previous = 0x07;
  const uint8_t current = (digitalRead(pins::kBoot) << 0) |
                          (digitalRead(pins::kVolumeDown) << 1) |
                          (digitalRead(pins::kVolumeUp) << 2);
  if (current != previous) {
    Serial.printf("BOOT=%s VOL-=%s VOL+=%s\n",
                  (current & 0x01) ? "released" : "PRESSED",
                  (current & 0x02) ? "released" : "PRESSED",
                  (current & 0x04) ? "released" : "PRESSED");
    previous = current;
  }
  delay(20);
}
