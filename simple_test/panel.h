#ifndef _FLIPDOT_PANEL_H
#define _FLIPDOT_PANEL_H

#include "Arduino.h"

#define DEFAULT_COL_PIN     8
#define DEFAULT_ROW_PIN     9
#define DEFAULT_COLOUR_PIN 10
#define DEFAULT_LATCH_PIN  11


/*
 * Flip Dot Panel (of unknown type) Driver
 * 
 * Things we know:
 * A line that takes pulses to select a column (reset row count)
 * A line that takes pulses to select a row
 * A line which sets pixel colour high/low
 * A line with latches the choice and resets row/col counts
 * 
 * so: set col, set row, set colour, pull the latch
 */
 
class FlipDotPanel
{
  public:
    FlipDotPanel(int pindelay=0, int colpin=DEFAULT_COL_PIN, int rowpin=DEFAULT_ROW_PIN,
                 int colourpin=DEFAULT_COLOUR_PIN, int latchpin=DEFAULT_LATCH_PIN);
                 
    void setdot(int row, int col, bool pen);

  private:
    int _pin_col;
    int _pin_row;
    int _pin_colour;
    int _pin_latch;
    int _pin_delay;
    void panel_init(void);
};


#endif
