#include <Adafruit_GFX.h>
#include "FlipDot_GFX.h"

FlipDot_GFX display;

void setup() {
  /* debug to serial port,
     use a vt100/ansi terminal for better debug */
  Serial.begin(115200);

  Serial.println("Panel Graphics Test");
  display.begin();
  display.setRotation(0);
  display.clearDisplay();
  display.refresh();
}

/* display random circles */
void circles(int count)
{
    uint8_t const w = display.width();
    uint8_t const h = display.height();
    uint8_t const maxr = min(w, h) / 2;

    display.clearDisplay();

    while (count > 0) {
        uint8_t x = random(w);
        uint8_t y = random(h);
        uint8_t r = random(2, maxr);

        display.drawCircle(x,y,r+1,0);
        display.fillCircle(x,y,r,1);
#ifdef DEBUG
        display.refresh();
        Serial.print("\x1B[0KCircles: ");
        Serial.print(count);
        Serial.print(" ("); Serial.print(x); Serial.print(","); Serial.print(y); Serial.print(") r="); Serial.println(r);
#else
        display.display();
#endif
        delay(100);
        count--;
    }
}

void rain(int count, int speed)
{
    uint8_t const h = display.height();
    while (count > 0) {
        display.scrollright();
        uint8_t row = random(h);
        display.drawPixel(0, row, 1);
#ifdef DEBUG
        display.refresh();
        Serial.print("\x1B[0KRain: ");
        Serial.print(count);
        Serial.print(" ");
        Serial.println(row);
#else
        display.display();
#endif
        count--;
        delay(speed);
    }
}

void scrolltext(String msg, int speed)
{
  uint8_t w = display.width();
  uint8_t twidth = (msg.length() * 6) + w;
  for(uint8_t x=0;x<twidth;x++) {
      display.clearDisplay();
      display.setCursor(w-x,4);
      display.print(msg);
#ifdef DEBUG
      Serial.println("\x1B[0KText: " + msg);
      display.refresh();
#else
      display.display();
#endif
      delay(speed);
  }
}

void waggle(String msg, int speed)
{
      display.clearDisplay();
      display.setCursor(0,4);
      display.print(msg);
      display.refresh();
      delay(1000);

      for (uint8_t i=0; i<3; i++) {
          display.scrolldown();
          display.refresh();
          delay(speed);
      }
      for (uint8_t i=0; i<6; i++) {
          display.scrollup();
          display.refresh();
          delay(speed);
      }
      for (uint8_t i=0; i<3; i++) {
          display.scrolldown();
          display.refresh();
          delay(speed);
      }
}

void loop() {
  display.clearDisplay();
  display.refresh();
  Serial.println("\x1B[0KText: Clear Panel");
  delay(1000);

  display.clearDisplay();
  display.drawRect(1, 2, 7, 7, BLACK);
  display.drawRect(2, 3, 5, 5, WHITE);
  display.display();
#ifdef DEBUG
  Serial.println("\x1B[0KDraw Rectangle");
  delay(1000);
  // show the full display just for the debug
  display.refresh(); 
#endif
  delay(1000);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  waggle("Hello", 200);
  delay(1000);

  scrolltext("This is Swansea Hackspace!", 100);
  delay(1000);

  display.clearDisplay();
  rain(30, 200);
  rain(30, 100);
  rain(60, 50);
  rain(60, 25);
  rain(200, 0);
  for (uint8_t i=0; i<display.width(); i++) {
      display.scrollright();
      display.display();
  }

  delay(3000);

  circles(100);
  delay(1000);
  
}
