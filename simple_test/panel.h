#ifndef _FLIPDOT_PANEL_H
#define _FLIPDOT_PANEL_H

#include "Arduino.h"

#define FLIPDOT_WIDTH               32
#define FLIPDOT_HEIGHT              16

#define FLIPDOT_COL_PIN             8
#define FLIPDOT_ROW_PIN             9
#define FLIPDOT_COLOUR_PIN          10
#define FLIPDOT_ENABLE_PIN          11
#define FLIPDOT_COMMIT_PIN          12

/* How long are reset/advance pulses (µS) */
#define PULSE_WIDTH     1

/* how long is the coil energise pulse (µS) */
#define COIL_PULSE      500

/*
 * Flip Dot Panel (of unknown type) Driver
 * derived from https://github.com/themainframe/esp32-flipdot-controller
 * 
 */
 
class FlipDotPanel
{
  public:
    FlipDotPanel();

    void enable();
    void disable();    
    void reset();
    void next_row();
    void next_col();
    void set_colour(bool pen);
    void commit();

    void setdot(uint8_t row, uint8_t col, bool pen);
};


#endif
