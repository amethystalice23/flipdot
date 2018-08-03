
#include "panel.h"

// Pin delay in microseconds sets size of pulses
FlipDotPanel::FlipDotPanel()
{
    pinMode(FLIPDOT_COL_PIN, OUTPUT);
    pinMode(FLIPDOT_ROW_PIN, OUTPUT);
    pinMode(FLIPDOT_COLOUR_PIN, OUTPUT);
    pinMode(FLIPDOT_ENABLE_PIN, OUTPUT);
    pinMode(FLIPDOT_COMMIT_PIN, OUTPUT);

    digitalWrite(FLIPDOT_COL_PIN, LOW);
    digitalWrite(FLIPDOT_ROW_PIN, LOW);
    digitalWrite(FLIPDOT_COLOUR_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE_PIN, LOW);
    digitalWrite(FLIPDOT_COMMIT_PIN, LOW);
}

void FlipDotPanel::enable()
{
    digitalWrite(FLIPDOT_ENABLE_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
}

void FlipDotPanel::disable()
{
    digitalWrite(FLIPDOT_ENABLE_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
}

/* reset the address lines */
void FlipDotPanel::reset()
{
    digitalWrite(FLIPDOT_COMMIT_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_COMMIT_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
}

void FlipDotPanel::next_row()
{
    digitalWrite(FLIPDOT_ROW_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_ROW_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
}

void FlipDotPanel::next_col()
{
    digitalWrite(FLIPDOT_COL_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_COL_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
}

void FlipDotPanel::set_colour(bool pen)
{
    digitalWrite(FLIPDOT_COLOUR_PIN, pen?HIGH:LOW);
}

void FlipDotPanel::commit()
{
//    digitalWrite(FLIPDOT_COIL_PIN, LOW);
    delayMicroseconds(COIL_PULSE);
//    digitalWrite(FLIPDOT_COIL_PIN, HIGH);
//    delayMicroseconds(COIL_PULSE);
}

/*
 * Set a single Pixel
 *
 * This operation is expensive, as it does a full reset and reposition
 * for every pixel, try not to use it.
 */
void FlipDotPanel::setdot(uint8_t row, uint8_t col, bool pen)
{
    enable();
    reset();
    set_colour(pen);
    for(uint8_t x=0; x<col; x++)
        next_col();
    for(uint8_t y=0; y<row; y++)
        next_row();
    commit();
    disable();
}
