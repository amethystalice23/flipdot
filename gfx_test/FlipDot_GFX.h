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
/* define DEBUG if you want a Serial port debugging of the panel output */
#undef DEBUG

/*=========================================================================*/


class FlipDot_GFX : public Adafruit_GFX {
 public:
  FlipDot_GFX();

  void begin();

  void clearDisplay(void);
  void display();
  void refresh();

  void scrollright();
  void scrollleft();
  void scrolldown();
  void scrollup();

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

 private:
  FlipDotPanel panel;

  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

};

#endif
