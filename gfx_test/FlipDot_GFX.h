/*********************************************************************
This is a library for Flipodot panels derived from the
Adafruit SSD1306 drivers
*********************************************************************/
#ifndef _FlipDot_GFX_H
#define _FlipDot_GFX_H

#include "Arduino.h"
#include "panel.h"

#include <Adafruit_GFX.h>

#define BLACK 0
#define WHITE 1
#define INVERSE 2

/*=========================================================================*/

#define FLIPDOT_WIDTH            32
#define FLIPDOT_HEIGHT           16


class FlipDot_GFX : public Adafruit_GFX {
 public:
  FlipDot_GFX(int pindelay, int8_t colpin, int8_t rowpin, int8_t inkpin, int8_t latchpin);
  FlipDot_GFX(int pindelay);

  void begin();

  void clearDisplay(void);
  void display();
  void refresh();

  void scrollright(uint8_t num);
  void scrollleft(uint8_t num);

  void scrolldown(uint8_t num);
  void scrollup(uint8_t num);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

 private:
  FlipDotPanel panel;

  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

};

#endif
