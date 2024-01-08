#pragma once

#include <Wire.h>

#define SCL_PIN 12
#define SDA_PIN 13

class tm1650_component : public Component {

public:
  void setup() override {
    Wire.begin(SDA_PIN, SCL_PIN);
    disp_updatecontrol();
  }

  void loop() override {}

  void update(char *numbers, bool column) {
    for (int i = 0; i < 4; i++) {
      uint8_t segment = 0;
      if (numbers[i] != ' ') {
        segment = disp_num[((int)numbers[i]) - 0x30];
      }
      if (column && (i==2 || i==3)) {
        segment |= 0x80;
      }

      Wire.beginTransmission(0x34 + i);
      Wire.write(segment);
      Wire.endTransmission();
    }
  }

  void set_intensity(int brightness) {
    if (brightness < 1)
      brightness = 1;
    if (brightness > 8)
      brightness = 8;
    disp_brightness = brightness;
    disp_updatecontrol();
  }

private:
  uint8_t disp_brightness = 8; // 1 (min) to 8 (max)
  uint8_t disp_mode7 = 0;      // 0 (8 segments) or 1 (7 segments)
  uint8_t disp_power = 1;      // 0 (off) or 1 (on)

  const uint8_t disp_num[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
  };

  int disp_updatecontrol() {
    int val =
        ((disp_brightness % 8) << 4) | (disp_mode7 << 3) | (disp_power << 0);
    Wire.beginTransmission(0x24); // register 0x48 DIG1CTRL
    Wire.write(val);
    return Wire.endTransmission();
  }
};
