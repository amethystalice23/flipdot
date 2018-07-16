
#include "panel.h"

// Pin delay in microseconds sets size of pulses
FlipDotPanel::FlipDotPanel(int pindelay, int colpin, int rowpin, int colourpin, int latchpin)
{
    _pin_delay = pindelay;
    _pin_col = colpin;
    _pin_row = rowpin;
    _pin_colour = colourpin;
    _pin_latch = latchpin;
    _pin_enable = ENABLE_PIN;

    pinMode(_pin_col, OUTPUT);
    pinMode(_pin_row, OUTPUT);
    pinMode(_pin_colour, OUTPUT);
    pinMode(_pin_latch, OUTPUT);
    pinMode(_pin_enable, OUTPUT);

    digitalWrite(_pin_enable, LOW);
}

void FlipDotPanel::setdot(int row, int col, bool pen)
{

      digitalWrite(_pin_enable, HIGH);

      // set the pen status ready
      digitalWrite(_pin_colour, pen?HIGH:LOW);
      
      // set column, as it reset row
      for (int i=0; i<col; i++) {
        digitalWrite(_pin_col, HIGH);
        if (PULSE_WIDTH > 0) delayMicroseconds(PULSE_WIDTH);
        digitalWrite(_pin_col, LOW);
        if (_pin_delay > 0) delayMicroseconds(_pin_delay);
      }

      // set row now it wont wander off
      for (int i=0; i<row; i++) {
        digitalWrite(_pin_row, HIGH);
        if (PULSE_WIDTH > 0) delayMicroseconds(PULSE_WIDTH);
        digitalWrite(_pin_row, LOW);
        if (_pin_delay > 0) delayMicroseconds(_pin_delay);
      }

      digitalWrite(_pin_enable, LOW);

      // and latch it
      digitalWrite(_pin_latch, LOW);
      if (PULSE_WIDTH > 0) delayMicroseconds(PULSE_WIDTH);
      digitalWrite(_pin_latch, HIGH);
      if (_pin_delay > 0) delayMicroseconds(_pin_delay);


}


