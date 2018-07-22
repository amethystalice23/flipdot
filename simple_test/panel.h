#ifndef _FLIPDOT_PANEL_H
#define _FLIPDOT_PANEL_H

#include "Arduino.h"

#define FLIPDOT_WIDTH               32
#define FLIPDOT_HEIGHT              16

#define FLIPDOT_COL_PIN             8
#define FLIPDOT_ROW_PIN             9
#define FLIPDOT_COLOUR_PIN          10
#define FLIPDOT_LATCH_PIN           11

#define FLIPDOT_ENABLE0_PIN         4
#define FLIPDOT_ENABLE1_PIN         5
#define FLIPDOT_ENABLE2_PIN         6
#define FLIPDOT_ENABLE3_PIN         7

#define PULSE_WIDTH 4

/*
 * Flip Dot Panel (of unknown type) Driver
 * 
 * Things we know:
 * A line that takes pulses to select a column (reset row count)
 * A line that takes pulses to select a row
 * A line which sets pixel colour high/low
 * A line with latches the choice
 * 
 * so: set col, set row, set colour, pull the latch
 */
 
class FlipDotPanel
{
  public:
    FlipDotPanel();
    
    void reset();
    void next_row();
    void next_col();
    void set_colour(bool pen);
    void commit();
    void next_panel();

    void setdot(uint8_t row, uint8_t col, bool pen);
};


#endif
