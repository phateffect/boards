#pragma once

#include <Arduino.h>
#include <SPI.h>

class OsptekSt7306 {
 public:
  static constexpr int16_t kWidth = 300;
  static constexpr int16_t kLogicalHeight = 200;

  enum class Color : uint8_t {
    kNeutral = 0x0,
    kRed = 0x3,
    kWhite = 0xC,
    kBlack = 0xF,
  };

  OsptekSt7306(SPIClass& spi, int8_t sclk, int8_t mosi, int8_t cs,
               int8_t dc, int8_t reset, int8_t te = -1);

  void begin(bool high_speed_mode = false);
  void clear(Color color);
  void drawPixel(int16_t x, int16_t y, Color color);
  void fillRect(int16_t x, int16_t y, int16_t width, int16_t height,
                Color color);
  void update();

 private:
  static constexpr size_t kBytesPerRow = kWidth / 2;
  static constexpr size_t kBufferSize = kBytesPerRow * kLogicalHeight;

  SPIClass& spi_;
  int8_t sclk_;
  int8_t mosi_;
  int8_t cs_;
  int8_t dc_;
  int8_t reset_;
  int8_t te_;
  uint32_t spi_frequency_ = 10000000;
  uint8_t buffer_[kBufferSize]{};

  void hardwareReset();
  void initializeController(bool high_speed_mode);
  void writeCommand(uint8_t command);
  void writeData(uint8_t data);
};
