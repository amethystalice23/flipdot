
#include "panel.h"

// Pin delay in microseconds sets size of pulses
FlipDotPanel::FlipDotPanel()
{
    pinMode(FLIPDOT_COL_PIN, OUTPUT);
    pinMode(FLIPDOT_ROW_PIN, OUTPUT);
    pinMode(FLIPDOT_COLOUR_PIN, OUTPUT);
    pinMode(FLIPDOT_LATCH_PIN, OUTPUT);

    pinMode(FLIPDOT_ENABLE0_PIN, OUTPUT);
    pinMode(FLIPDOT_ENABLE1_PIN, OUTPUT);
    pinMode(FLIPDOT_ENABLE2_PIN, OUTPUT);
    pinMode(FLIPDOT_ENABLE3_PIN, OUTPUT);

    digitalWrite(FLIPDOT_COL_PIN, LOW);
    digitalWrite(FLIPDOT_ROW_PIN, LOW);
    digitalWrite(FLIPDOT_COLOUR_PIN, LOW);
    digitalWrite(FLIPDOT_LATCH_PIN, LOW);

    digitalWrite(FLIPDOT_ENABLE0_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE1_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE2_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE3_PIN, LOW);
}

/* sequence to reset the address lines */
void FlipDotPanel::reset()
{
    digitalWrite(FLIPDOT_ENABLE0_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE1_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE2_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE3_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_COL_PIN, HIGH);
    digitalWrite(FLIPDOT_LATCH_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_ENABLE0_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE1_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE2_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE3_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_COL_PIN, LOW);
    digitalWrite(FLIPDOT_LATCH_PIN, LOW);
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
    digitalWrite(FLIPDOT_ENABLE0_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE1_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE2_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE3_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_LATCH_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_ENABLE1_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE2_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE3_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_LATCH_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE0_PIN, HIGH);
}

/* dont actually know what this does, it might not
 * be a panel change, but it does occur in the traces
 */
void FlipDotPanel::next_panel()
{
    digitalWrite(FLIPDOT_LATCH_PIN, HIGH);
    digitalWrite(FLIPDOT_COL_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);

    digitalWrite(FLIPDOT_ENABLE0_PIN, LOW);
    digitalWrite(FLIPDOT_ENABLE1_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE2_PIN, HIGH);
    digitalWrite(FLIPDOT_ENABLE3_PIN, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(FLIPDOT_COL_PIN, LOW);
    digitalWrite(FLIPDOT_LATCH_PIN, LOW);
    delayMicroseconds(PULSE_WIDTH);
    delayMicroseconds(PULSE_WIDTH);
    delayMicroseconds(PULSE_WIDTH);
    reset();
}

/*
 * Set a single Pixel
 *
 * This operation is expensive, as it does a full reset and reposition
 * for every pixel, try not to use it.
 */
void FlipDotPanel::setdot(uint8_t row, uint8_t col, bool pen)
{
    reset();
    set_colour(pen);
    for(uint8_t x=0; x<col; x++)
        next_col();
    for(uint8_t y=0; y<row; y++)
        next_row();
    commit();
}
