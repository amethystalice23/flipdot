/*********************************************************************
 * This is a library for Flipodot panels derived from the
 * Adafruit SSD1306 drivers

*********************************************************************/

#if !defined(__ARM_ARCH) && !defined(ENERGIA) && !defined(ESP8266) && !defined(ESP32) && !defined(__arc__)
 #include <util/delay.h>
#endif

#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "FlipDot_GFX.h"
#include "panel.h"

// the memory buffer for the  panel
static uint8_t buffer[FLIPDOT_HEIGHT * FLIPDOT_WIDTH / 8] = { 0 };

// the memory of the previous buffer, ie whats displayed now
static uint8_t memory[FLIPDOT_HEIGHT * FLIPDOT_WIDTH / 8] = { 0 };


#define flipdot_swap(a, b) { int16_t t = a; a = b; b = t; }

// the most basic function, set a single pixel
void FlipDot_GFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    flipdot_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    flipdot_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  // x is which column
    switch (color)
    {
      case WHITE:   buffer[x+ (y/8)*FLIPDOT_WIDTH] |=  (1 << (y&7)); break;
      case BLACK:   buffer[x+ (y/8)*FLIPDOT_WIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*FLIPDOT_WIDTH] ^=  (1 << (y&7)); break;
    }

}

// initializer if you want to specify all the pins
FlipDot_GFX::FlipDot_GFX(int pd, int8_t cp, int8_t rp, int8_t ip, int8_t lp) 
: Adafruit_GFX(FLIPDOT_WIDTH, FLIPDOT_HEIGHT)
, panel(pd, cp, rp, ip, lp) {
}

FlipDot_GFX::FlipDot_GFX(int pd) 
: Adafruit_GFX(FLIPDOT_WIDTH, FLIPDOT_HEIGHT)
, panel(pd) {
}


void FlipDot_GFX::begin() {
  // Init sequence
  memset(buffer, 0, (FLIPDOT_WIDTH*FLIPDOT_HEIGHT/8));
  memset(memory, 0, (FLIPDOT_WIDTH*FLIPDOT_HEIGHT/8));  
}


// scrollright - scroll whole panel right 'num' pixels
void FlipDot_GFX::scrollright(uint8_t num ){
    //TODO
}

// scrollleft - scroll whole panel left 'num' pixels
void FlipDot_GFX::scrollleft(uint8_t num ){
    //TODO
}

// scrollup - scroll whole panel up 'num' pixels
void FlipDot_GFX::scrollup(uint8_t num ){
    //TODO
}

// scrolldown - scroll whole panel down 'num' pixels
void FlipDot_GFX::scrolldown(uint8_t num ){
    //TODO
}

void FlipDot_GFX::refresh(void) {
    memcpy(memory, buffer, FLIPDOT_WIDTH * (FLIPDOT_HEIGHT/8));
    // Just update everything
    Serial.println("Flipdot. refresh display");
    for (uint8_t y=0; y<FLIPDOT_HEIGHT; y++) {
        String line = y<10?"0":"";
        line += (String)y;
        line += ": ";
        
        for (uint8_t x=0; x<FLIPDOT_WIDTH; x++) {
            uint8_t block = memory[x + (y/8)*FLIPDOT_WIDTH];
            uint8_t patt = 1<<(y&7);

            panel.setdot(y, x, block & patt?1:0);
            line += block & patt?"O ":"| ";
        }
        Serial.println(line);
    }
}

void FlipDot_GFX::display(void) {
    //Work out what has changed between now and last time and send draw commands
    Serial.println("Flipdot. commit display");
    for (uint8_t y=0; y<FLIPDOT_HEIGHT; y++) {
        String line = y<10?"0":"";
        line += (String)y;
        line += ": ";
        for (uint8_t x=0; x<FLIPDOT_WIDTH; x++) {
            uint8_t block = buffer[x + (y/8)*FLIPDOT_WIDTH];
            uint8_t old = memory[x + (y/8)*FLIPDOT_WIDTH];
            uint8_t patt = 1<<(y&7);

                if (block & patt != old & patt) {
                    panel.setdot(y, x, block & patt?1:0);
                    line += block & patt?"O ":"| ";
                } else {
                    line += ". ";
                }
        }
        Serial.println(line);
    }
    memcpy(memory, buffer, FLIPDOT_WIDTH * (FLIPDOT_HEIGHT/8));
}

// clear everything
void FlipDot_GFX::clearDisplay(void) {
  memset(buffer, 0, (FLIPDOT_WIDTH*FLIPDOT_HEIGHT/8));
}


void FlipDot_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  boolean bSwap = false;
  switch(rotation) {
    case 0:
      // 0 degree rotation, do nothing
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x
      bSwap = true;
      flipdot_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      x -= (w-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
      bSwap = true;
      flipdot_swap(x, y);
      y = HEIGHT - y - 1;
      y -= (w-1);
      break;
  }

  if(bSwap) {
    drawFastVLineInternal(x, y, w, color);
  } else {
    drawFastHLineInternal(x, y, w, color);
  }
}

void FlipDot_GFX::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Do bounds/limit checks
  if(y < 0 || y >= HEIGHT) { return; }

  // make sure we don't try to draw below 0
  if(x < 0) {
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if( (x + w) > WIDTH) {
    w = (WIDTH - x);
  }

  // if our width is now negative, punt
  if(w <= 0) { return; }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * FLIPDOT_WIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y&7);

  switch (color)
  {
  case WHITE:         while(w--) { *pBuf++ |= mask; }; break;
    case BLACK: mask = ~mask;   while(w--) { *pBuf++ &= mask; }; break;
  case INVERSE:         while(w--) { *pBuf++ ^= mask; }; break;
  }
}

void FlipDot_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  bool bSwap = false;
  switch(rotation) {
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      flipdot_swap(x, y);
      x = WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y
      bSwap = true;
      flipdot_swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  if(bSwap) {
    drawFastHLineInternal(x, y, h, color);
  } else {
    drawFastVLineInternal(x, y, h, color);
  }
}


void FlipDot_GFX::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  }

  // make sure we don't go past the height of the display
  if( (__y + __h) > HEIGHT) {
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt
  if(__h <= 0) {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * FLIPDOT_WIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) {
      mask &= (0XFF >> (mod-h));
    }

  switch (color)
    {
    case WHITE:   *pBuf |=  mask;  break;
    case BLACK:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += FLIPDOT_WIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) {
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += FLIPDOT_WIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += FLIPDOT_WIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color)
    {
      case WHITE:   *pBuf |=  mask;  break;
      case BLACK:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
}