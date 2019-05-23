
void ShowNull() {
  for (int i = 0; i < LEDS_COUNT; i++) {
    Display.set_crgb_at(i, nPixel);
  }
  Display.sync();
}

void ShowShortage() {
  if (millis() - time_animation > DELAY_ANIMATION) {
    time_animation = millis();
    
    for (int i = 0; i < LEDS_COUNT; i++) {
      Display.set_crgb_at(i, gPixel);
    }
    Display.sync();
  }
}

void ShowVoltage(int voltage, bool positive) {
  if (millis() - time_animation > DELAY_ANIMATION) {
    time_animation = millis();

    for (int i = 0; i < LEDS_COUNT; i++) {
      Display.set_crgb_at(i, nPixel);
    }

    cRGB pixel;

    byte num = voltage >> 7; // 10 bit
    for (uint8_t i = 0; i <= num / 2; i++) {
      pixel.r = positive ? LIGHTNESS : 0;
      pixel.b = positive ? 0 : LIGHTNESS;
      Display.set_crgb_at(i, pixel);
    }

    for (uint8_t i = num / 2 + 1; i < num; i++) {
      pixel.b = positive ? LIGHTNESS : 0;
      pixel.r = positive ? 0 : LIGHTNESS;
      Display.set_crgb_at(i, pixel);
    }

    pixel.b = positive ? (voltage % 128) : 0;
    pixel.r = positive ? 0 : (voltage % 128);
    Display.set_crgb_at(num, pixel);

    Display.sync();

  }
}

void ShowIntro() {
  for (byte timeline = 0; timeline < 255; timeline++) {

    for (byte i = 0; i < LEDS_COUNT; i++) {
      Display.set_crgb_at(i, Wheel(timeline));
    }

    Display.sync();
    delay(10);
  }
}

cRGB Wheel(byte pos) {
  cRGB pixel;

  pos = 255 - pos;
  if (pos < 85) {
    pixel.r = 255 - pos * 3;
    pixel.g = 0;
    pixel.b = pos * 3;
    return pixel;
  }
  if (pos < 170) {
    pos -= 85;
    pixel.r = 0;
    pixel.g = pos * 3;
    pixel.b = 255 - pos * 3;
    return pixel;
  }
  pos -= 170;
  pixel.r = pos * 3;
  pixel.g = 255 - pos * 3;
  pixel.b = 0;
  return pixel;

}