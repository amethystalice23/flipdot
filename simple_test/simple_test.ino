#include "panel.h"

// create a panel, use default pin assignments
FlipDotPanel panel;

void setup() {
  Serial.begin(115200);

  Serial.println('Simple Panel Test');
}


void clear_all(int speed)
{
  Serial.println("Second test. clear all slowly");
  panel.enable();
  panel.reset();
  panel.set_colour(0);
  for(int col=0; col<FLIPDOT_WIDTH; col++) {
    for (int row=0;row<FLIPDOT_HEIGHT; row++) {
        panel.commit();
        panel.next_row();
        delay(speed);   // SLOW it down so we can see it working
    }
    panel.next_col();
  }
  panel.disable();
  
}
void set_all(int speed)
{
  panel.enable();
  panel.reset();
  panel.set_colour(1);
  for(int col=0; col<FLIPDOT_WIDTH; col++) {
    for (int row=0;row<FLIPDOT_HEIGHT; row++) {
        panel.commit();
        panel.next_row();
        delay(speed);   // SLOW it down so we can see it working
    }
    panel.next_col();
  }
  panel.disable();
}

void snake(int speed)
{
  int xa=0, xb=0, ya=0, yb=0;
  int stepdir = 1;
 
  do {
    panel.setdot(yb, xb, 0);
    yb=ya;
    xb=xa;

    xa += stepdir;
    if (xa < 1) {
      xa = 1;
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
    delay(speed);   // SLOW it down so we can see it working
  }while (1);
}
void loop() {
  clear_all(0);
  set_all(50);
  delay(1000);
  clear_all(10);
  delay(1000);
  snake(10);
  snake(50);
}
