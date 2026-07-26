#include "osptek_st7306.h"

#include <cstring>

OsptekSt7306::OsptekSt7306(SPIClass& spi, int8_t sclk, int8_t mosi,
                           int8_t cs, int8_t dc, int8_t reset, int8_t te)
    : spi_(spi),
      sclk_(sclk),
      mosi_(mosi),
      cs_(cs),
      dc_(dc),
      reset_(reset),
      te_(te) {}

void OsptekSt7306::begin(bool high_speed_mode) {
  pinMode(cs_, OUTPUT);
  pinMode(dc_, OUTPUT);
  pinMode(reset_, OUTPUT);
  if (te_ >= 0) {
    pinMode(te_, INPUT);
  }
  digitalWrite(cs_, HIGH);

  spi_frequency_ = high_speed_mode ? 20000000 : 10000000;
  spi_.begin(sclk_, -1, mosi_, cs_);
  initializeController(high_speed_mode);
}

void OsptekSt7306::hardwareReset() {
  digitalWrite(reset_, HIGH);
  delay(20);
  digitalWrite(reset_, LOW);
  delay(50);
  digitalWrite(reset_, HIGH);
  delay(150);
}

void OsptekSt7306::writeCommand(uint8_t command) {
  spi_.beginTransaction(
      SPISettings(spi_frequency_, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_, LOW);
  digitalWrite(cs_, LOW);
  spi_.transfer(command);
  digitalWrite(cs_, HIGH);
  spi_.endTransaction();
}

void OsptekSt7306::writeData(uint8_t data) {
  spi_.beginTransaction(
      SPISettings(spi_frequency_, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_, HIGH);
  digitalWrite(cs_, LOW);
  spi_.transfer(data);
  digitalWrite(cs_, HIGH);
  spi_.endTransaction();
}

void OsptekSt7306::initializeController(bool high_speed_mode) {
  hardwareReset();

  writeCommand(0xD6);
  writeData(0x17);
  writeData(0x02);
  writeCommand(0xD1);
  writeData(0x01);
  writeCommand(0xC0);
  writeData(0x08);
  writeData(0x00);

  writeCommand(0xC1);
  for (int i = 0; i < 4; ++i) writeData(0x2C);
  writeCommand(0xC2);
  for (int i = 0; i < 4; ++i) writeData(0x26);
  writeCommand(0xC4);
  for (int i = 0; i < 4; ++i) writeData(0x42);
  writeCommand(0xC5);
  for (int i = 0; i < 4; ++i) writeData(0x32);

  if (high_speed_mode) {
    writeCommand(0xD8);
    writeData(0xA6);
    writeData(0xE9);
    writeCommand(0xB2);
    writeData(0x02);
  } else {
    writeCommand(0xB2);
    writeData(0x12);
  }

  constexpr uint8_t kWaveformB3[] = {
      0xE5, 0xF6, 0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45};
  constexpr uint8_t kWaveformB4[] = {
      0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45};
  writeCommand(0xB3);
  for (uint8_t value : kWaveformB3) writeData(value);
  writeCommand(0xB4);
  for (uint8_t value : kWaveformB4) writeData(value);

  writeCommand(0xB7);
  writeData(0x13);
  writeCommand(0xB0);
  writeData(0x64);
  writeCommand(0xC9);
  writeData(0x00);
  writeCommand(0x11);
  delay(255);
  writeCommand(0x36);
  writeData(0x80);
  writeCommand(0x3A);
  writeData(0x11);
  writeCommand(0xB9);
  writeData(0x20);
  writeCommand(0xB8);
  writeData(0x09);
  writeCommand(0xD0);
  writeData(0xFF);
  writeCommand(0x72);
  writeData(0x13);
  writeCommand(high_speed_mode ? 0x38 : 0x39);
  writeCommand(0x29);
}

void OsptekSt7306::clear(Color color) {
  const uint8_t nibble = static_cast<uint8_t>(color) & 0x0F;
  std::memset(buffer_, static_cast<uint8_t>((nibble << 4) | nibble),
              sizeof(buffer_));
}

void OsptekSt7306::drawPixel(int16_t x, int16_t y, Color color) {
  if (x < 0 || x >= kWidth || y < 0 || y >= kLogicalHeight) {
    return;
  }
  const size_t index = static_cast<size_t>(y) * kBytesPerRow + x / 2;
  const uint8_t nibble = static_cast<uint8_t>(color) & 0x0F;
  if ((x & 1) == 0) {
    buffer_[index] = (buffer_[index] & 0x0F) | (nibble << 4);
  } else {
    buffer_[index] = (buffer_[index] & 0xF0) | nibble;
  }
}

void OsptekSt7306::fillRect(int16_t x, int16_t y, int16_t width,
                            int16_t height, Color color) {
  for (int16_t row = y; row < y + height; ++row) {
    for (int16_t column = x; column < x + width; ++column) {
      drawPixel(column, row, color);
    }
  }
}

void OsptekSt7306::update() {
  writeCommand(0x2A);
  writeData(0x05);
  writeData(0x36);
  writeCommand(0x2B);
  writeData(0x00);
  writeData(0xC7);
  writeCommand(0x2C);

  spi_.beginTransaction(
      SPISettings(spi_frequency_, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_, HIGH);
  digitalWrite(cs_, LOW);
  constexpr size_t kChunkSize = 512;
  for (size_t offset = 0; offset < sizeof(buffer_); offset += kChunkSize) {
    const size_t remaining = sizeof(buffer_) - offset;
    const size_t length =
        remaining < kChunkSize ? remaining : kChunkSize;
    spi_.transferBytes(buffer_ + offset, nullptr, length);
  }
  digitalWrite(cs_, HIGH);
  spi_.endTransaction();
}
