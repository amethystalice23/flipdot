#include "panel.h"

// create a panel, use default pin assignments
FlipDotPanel panel;

void setup() {
  Serial.begin(115200);

  Serial.println('Simple Panel Test');
}

void loop() {
  Serial.println('First test, all set');

  panel.reset();
  panel.set_colour(1);
  for(int col=0; col<FLIPDOT_WIDTH; col++) {
    for (int row=0;row<FLIPDOT_HEIGHT; row++) {
        panel.commit();
        panel.next_row();
    }
    panel.next_col();
  }

  Serial.println("Pausing");
  delay(2000);
  panel.reset();
  panel.set_colour(0);
  for(int col=0; col<FLIPDOT_WIDTH; col++) {
    for (int row=0;row<FLIPDOT_HEIGHT; row++) {
        panel.commit();
        panel.next_row();
    }
    panel.next_col();
  }

  Serial.println("Second test. all blank");
  Serial.println("Pausing");
  delay(2000);

  Serial.println("Running dot test");
  int xa=0, xb=0, ya=0, yb=0;
  int stepdir = 1;
  
  do {
    panel.setdot(yb, xb, 0);
    yb=ya;
    xb=xa;

    xa += stepdir;
    if (xa < 0) {
      xa = 0;
      stepdir = 1;
      ya++;
    } else
    if (xa >= FLIPDOT_WIDTH) {
      xa = FLIPDOT_WIDTH - 1;
      stepdir = -1;
      ya++;
    }

    if (ya >= FLIPDOT_HEIGHT) {
      break;
    }
    panel.setdot(ya, xa, 1);
  }while (1);
  delay(2000);
}
