////////////////////////////////////////////////////////////////////////////////////////
//
//      2021 Georgi Angelov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ST7789_H_
#define _ST7789_H_

#include <Arduino.h>
#include <Print.h>
#include <Adafruit_GFX.h>
#include "hal_tft.h"

#define ST7789_TFTWIDTH TFT_WIDTH
#define ST7789_TFTHEIGHT TFT_HEIGHT

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0

#define ST_CMD_DELAY 0x80

#define ST7789_NOP 0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID 0x04
#define ST7789_RDDST 0x09

#define ST7789_SLPIN 0x10
#define ST7789_SLPOUT 0x11
#define ST7789_PTLON 0x12
#define ST7789_NORON 0x13

#define ST7789_INVOFF 0x20
#define ST7789_INVON 0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON 0x29
#define ST7789_CASET 0x2A
#define ST7789_RASET 0x2B
#define ST7789_RAMWR 0x2C
#define ST7789_RAMRD 0x2E

#define ST7789_PTLAR 0x30
#define ST7789_COLMOD 0x3A
#define ST7789_MADCTL 0x36

#define ST7789_IDMOFF 0x38
#define ST7789_IDMON 0x39
#define ST7789_VSCRDEF 0x33
#define ST7789_VSCRSADD 0x37

#define ST7789_MADCTL_MY 0x80
#define ST7789_MADCTL_MX 0x40
#define ST7789_MADCTL_MV 0x20
#define ST7789_MADCTL_ML 0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1 0xDA
#define ST7789_RDID2 0xDB
#define ST7789_RDID3 0xDC
#define ST7789_RDID4 0xDD

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

static const uint8_t
    ST7789_240x240[] = {
        9,                                                 // 9 commands in list:
        ST7789_SWRESET, ST_CMD_DELAY,                      // 1: Software reset, no args, w/delay
        150,                                               // 150 ms delay
        ST7789_SLPOUT, ST_CMD_DELAY,                       // 2: Out of sleep mode, no args, w/delay
        255,                                               // 255 = 500 ms delay
        ST7789_COLMOD, 1 + ST_CMD_DELAY,                   // 3: Set color mode, 1 arg + delay:
        0x55,                                              // 16-bit color
        10,                                                // 10 ms delay
        ST7789_MADCTL, 1,                                  // 4: Memory access ctrl (directions), 1 arg:
        0x00,                                              // Row addr/col addr, bottom to top refresh
        ST7789_CASET, 4,                                   // 5: Column addr set, 4 args, no delay:
        0x00, ST7789_240x240_XSTART,                       // XSTART = 0
        (ST7789_TFTWIDTH + ST7789_240x240_XSTART) >> 8,    //
        (ST7789_TFTWIDTH + ST7789_240x240_XSTART) & 0xFF,  // XEND = 240
        ST7789_RASET, 4,                                   // 6: Row addr set, 4 args, no delay:
        0x00, ST7789_240x240_YSTART,                       // YSTART = 0
        (ST7789_TFTHEIGHT + ST7789_240x240_YSTART) >> 8,   //
        (ST7789_TFTHEIGHT + ST7789_240x240_YSTART) & 0xFF, // YEND = 240
        ST7789_INVON, ST_CMD_DELAY,                        // 7: Inversion ON
        10,                                                //
        ST7789_NORON, ST_CMD_DELAY,                        // 8: Normal display on, no args, w/delay
        10,                                                // 10 ms delay
        ST7789_DISPON, ST_CMD_DELAY,                       // 9: Main screen turn on, no args, w/delay
        20};

class Arduino_ST7789 : public Adafruit_GFX
{

public:
  Arduino_ST7789()
      : Adafruit_GFX(ST7789_TFTWIDTH, ST7789_TFTHEIGHT)
  {
    _ystart = _xstart = _colstart = _rowstart = 0;
  }

  void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
  {
    xs += _xstart;
    xe += _xstart;
    ys += _ystart;
    ye += _ystart;

    TFT_DC_CMD();
    TFT_CS_ENABLE();
    tft_spi_put(ST7789_CASET, 8);

    TFT_DC_DATA();
    tft_spi_put(xs, 16);
    tft_spi_put(xe, 16);

    TFT_DC_CMD();
    tft_spi_put(ST7789_RASET, 8);

    TFT_DC_DATA();
    tft_spi_put(ys, 16);
    tft_spi_put(ye, 16);

    TFT_DC_CMD();
    tft_spi_put(ST7789_RAMWR, 8);

    TFT_DC_DATA();
  }

  void pushColor(uint16_t color)
  {
    TFT_DC_DATA();
    TFT_CS_ENABLE();
    tft_spi_put(color, 8);
    TFT_CS_DISABLE();
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
      return;
    setAddrWindow(x, y, x + 1, y + 1);
    tft_spi_put(color, 16);
    TFT_CS_DISABLE();
  }

  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    if ((x >= _width) || (y >= _height))
      return;
    if ((y + h - 1) >= _height)
      h = _height - y;
    setAddrWindow(x, y, x, y + h - 1);
    tft_spi_put_data(color, h, 16);
    TFT_CS_DISABLE();
  }

  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    if ((x >= _width) || (y >= _height))
      return;
    if ((x + w - 1) >= _width)
      w = _width - x;
    setAddrWindow(x, y, x + w - 1, y);
    tft_spi_put_data(color, w, 16);
    TFT_CS_DISABLE();
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
  {
    if ((x >= _width) || (y >= _height))
      return;
    if ((x + w - 1) >= _width)
      w = _width - x;
    if ((y + h - 1) >= _height)
      h = _height - y;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    tft_spi_put_data(color, w * h, 16);
    TFT_CS_DISABLE();
  }

  void drawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img)
  {

    if (w <= 0 || h <= 0)
      return;
    uint16_t *buffer = (uint16_t *)img;
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    tft_spi_put_buffer(buffer, w * h, 16);
    TFT_CS_DISABLE();
  }
  //inline void drawImageF(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img) { drawImage(x, y, w, h, img); }

  void init(uint16_t width, uint16_t height)
  {
    _width = width;
    _height = height;

    if (width == 240 && height == 240)
      _rowstart = 80;

    tft_init();
    tft_list_init(ST7789_240x240);
    setRotation(2);
  }

  inline void fillScreen(uint16_t color) { fillRect(0, 0, _width, _height, color); }

  inline void invertDisplay(bool mode) { tft_write_cmd(mode ? ST7789_INVON : ST7789_INVOFF); }

  inline void partialDisplay(bool mode) { tft_write_cmd(mode ? ST7789_PTLON : ST7789_NORON); }

  inline void enableDisplay(bool mode) { tft_write_cmd(mode ? ST7789_DISPON : ST7789_DISPOFF); }

  inline void idleDisplay(bool mode) { tft_write_cmd(mode ? ST7789_IDMON : ST7789_IDMOFF); }

  inline void resetDisplay()
  {
    tft_write_cmd(ST7789_SWRESET);
    delay(5);
  }

  inline void sleepDisplay(bool mode)
  {
    tft_write_cmd(mode ? ST7789_SLPIN : ST7789_SLPOUT);
    delay(5);
  }

  void setScrollArea(uint16_t tfa, uint16_t bfa)
  {
    uint16_t vsa = 320 - tfa - bfa;
    tft_write_cmd(ST7789_VSCRDEF);
    tft_spi_put(tfa, 16);
    tft_spi_put(vsa, 16);
    tft_spi_put(bfa, 16);
  }

  void setScroll(uint16_t vsp)
  {
    tft_write_cmd(ST7789_VSCRSADD);
    tft_spi_put(vsp, 16);
  }

  void setPartArea(uint16_t sr, uint16_t er)
  {
    tft_write_cmd(ST7789_PTLAR);
    tft_spi_put(sr, 16);
    tft_spi_put(er, 16);
  }

  void setRotation(uint8_t m)
  {
    tft_write_cmd(ST7789_MADCTL);
    rotation = m % 4; // can't be higher than 3
    switch (rotation)
    {
    case 0:
      tft_write_data(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
      _xstart = _colstart;
      _ystart = _rowstart;
      break;
    case 1:
      tft_write_data(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
      _ystart = _colstart;
      _xstart = _rowstart;
      break;
    case 2:
      tft_write_data(ST7789_MADCTL_RGB);
      _xstart = 0;
      _ystart = 0;
      break;
    case 3:
      tft_write_data(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
      _xstart = 0;
      _ystart = 0;
      break;
    }
  }

  inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }
  inline uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); }

private:
  uint8_t _colstart, _rowstart, _xstart, _ystart;
};

#endif