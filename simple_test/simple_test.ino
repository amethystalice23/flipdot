#include "panel.h"

#define ROW_LIMIT 16
#define COL_LIMIT 32

// create a panel, use default pin assignments
FlipDotPanel panel(400);

void setup() {
  Serial.begin(115200);

  Serial.println('Simple Panel Test');
}

void loop() {
  Serial.println('First test, all set');
  for (int row=0;row<ROW_LIMIT; row++) {
    for(int col=0; col<COL_LIMIT; col++) {
      panel.setdot(row, col, 1);
    }
  }
  Serial.println("Pause 1 second");
  delay(2000);

  Serial.println("Second test. all blank");
  for (int row=0;row<ROW_LIMIT; row++) {
    for(int col=0; col<COL_LIMIT; col++) {
      panel.setdot(row, col, 0);
    }
  }
  Serial.println("Pause 1 second");
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
    if (xa >= COL_LIMIT) {
      xa = COL_LIMIT - 1;
      stepdir = -1;
      ya++;
    }

    if (ya >= ROW_LIMIT) {
      break;
    }
    panel.setdot(ya, xa, 1);
  }while (1);
  delay(2000);
}
