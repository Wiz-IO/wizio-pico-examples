#pragma once

#include "st7789.hpp"
#include "pico_graphics.hpp"

namespace pimoroni {

  class PicoDisplay : public PicoGraphics {
  public:
    static const int WIDTH = 240;
    static const int HEIGHT = 135;
    static const uint8_t A = 12;
    static const uint8_t B = 13;
    static const uint8_t X = 14;
    static const uint8_t Y = 15;

    uint16_t *__fb;
  private:
    ST7789 screen;

  public:
    PicoDisplay(uint16_t *buf);

    void init();
    void update();
    void set_backlight(uint8_t brightness);
    void set_led(uint8_t r, uint8_t g, uint8_t b);
    bool is_pressed(uint8_t button);
  };

}