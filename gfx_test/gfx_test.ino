#include <Adafruit_GFX.h>
#include "FlipDot_GFX.h"


FlipDot_GFX display(0);

void setup() {
  Serial.begin(115200);

  Serial.println("Panel Graphics Test");
  display.begin();
  display.clearDisplay();
  display.display();
  display.refresh();
}

void loop() {
  delay(5000);

  display.clearDisplay();
  Serial.println("Draw rectangle");
  display.drawRect(1, 2, 7, 7, BLACK);
  display.drawRect(2, 3, 5, 5, WHITE);
  display.refresh();

  delay(5000);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,3);
  display.clearDisplay();
  display.println("Hello");
  display.refresh();
 
  delay(10000);

}
