#include <Arduino.h>
#include <SPI.h>

#include "osptek_st7306.h"

#ifndef OSPTEK_PIN_SCLK
#define OSPTEK_PIN_SCLK 12
#endif
#ifndef OSPTEK_PIN_MOSI
#define OSPTEK_PIN_MOSI 11
#endif
#ifndef OSPTEK_PIN_CS
#define OSPTEK_PIN_CS 10
#endif
#ifndef OSPTEK_PIN_DC
#define OSPTEK_PIN_DC 9
#endif
#ifndef OSPTEK_PIN_RESET
#define OSPTEK_PIN_RESET 4
#endif
#ifndef OSPTEK_PIN_TE
#define OSPTEK_PIN_TE 5
#endif

// The initialization sequence and color encoding are distilled from the
// archived MIT-licensed reference driver. Pins are build-time configurable,
// and the framebuffer API exposes the controller's 300x200 logical scan grid.
SPIClass display_spi(HSPI);
OsptekSt7306 display(display_spi, OSPTEK_PIN_SCLK, OSPTEK_PIN_MOSI,
                     OSPTEK_PIN_CS, OSPTEK_PIN_DC, OSPTEK_PIN_RESET,
                     OSPTEK_PIN_TE);

void drawTestPattern() {
  using Color = OsptekSt7306::Color;
  display.clear(Color::kWhite);

  display.fillRect(10, 10, 85, 80, Color::kBlack);
  display.fillRect(107, 10, 86, 80, Color::kRed);
  display.fillRect(205, 10, 85, 80, Color::kNeutral);

  for (int16_t x = 10; x < OsptekSt7306::kWidth - 10; x += 12) {
    display.fillRect(x, 110, 6, 70,
                     (x / 12) % 2 ? Color::kBlack : Color::kRed);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Initializing Osptek YDP420H003-V3...");

  // false: documented 1 Hz low-power path; true selects the experimental
  // 16 Hz high-speed settings from the reference implementation.
  display.begin(false);
  drawTestPattern();
  display.update();
  Serial.println("Test pattern sent.");
}

void loop() {
  delay(1000);
}
