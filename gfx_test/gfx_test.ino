#include <Adafruit_GFX.h>
#include "FlipDot_GFX.h"


FlipDot_GFX display;

void setup() {
  /* debug to serial port,
     use a vt100/ansi terminal for better debug */
  Serial.begin(115200);

  Serial.println("Panel Graphics Test");
  display.begin();
  display.clearDisplay();
  display.refresh();
}

void loop() {
  display.clearDisplay();
  display.refresh();
  Serial.println("\x1B[0KText: Clear Panel");
  delay(5000);

  display.clearDisplay();
  display.drawRect(1, 2, 7, 7, BLACK);
  display.drawRect(2, 3, 5, 5, WHITE);
  display.display();
  Serial.println("\x1B[0KDraw Rectangle");
  delay(5000);
  // show the full display just for the debug
  display.refresh(); 
  delay(3000);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,3);
  display.clearDisplay();
  display.print("Hello");
  display.display();
  Serial.println("\x1B[0KText: Hello");
  delay(5000);
  // show the full display just for the debug
  display.refresh();
  delay(3000);

}
