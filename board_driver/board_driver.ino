#include <Adafruit_GFX.h>
#include "FlipDot_GFX.h"

FlipDot_GFX display;

/* Driver address pins (w/ pullups) */
#define PANELADDR_0         2
#define PANELADDR_1         3
#define PANELADDR_2         4
#define PANELADDR_3         5

uint8_t read_paneladdr(void)
{
    uint8_t num = 0;

    if (digitalRead(PANELADDR_0) == HIGH) num |= 0x01;
    if (digitalRead(PANELADDR_1) == HIGH) num |= 0x02;
    if (digitalRead(PANELADDR_2) == HIGH) num |= 0x04;
    if (digitalRead(PANELADDR_3) == HIGH) num |= 0x08;
    return num;
}

/* falling rain effect */
void rain(int count, int speed)
{
    uint8_t const h = display.height();
    while (count > 0) {
        display.scrollright();
        uint8_t row = random(h);
        display.drawPixel(0, row, 1);
        display.display();
        count--;
        delay(speed);
    }
}

/* scroll some text
 * call with frame==-1 to obtain number of frames required
 * then call with frame == 0..N
 */
int scrolltext(String msg, int frame)
{
  uint8_t w = display.width();
  int twidth = (msg.length() * 6) + w;

  if (frame == -1) return twidth;

  display.clearDisplay();
  display.setCursor(w-frame,4);
  display.print(msg);
  display.display();
}

/* just plain draw text */
void show_text(String text)
{
  display.clearDisplay();
  display.setCursor(0,4);
  display.print(text);
  display.display();
}

unsigned int readHex(String hex)
{
    unsigned int val = 0;
    for (uint8_t i=0; i<hex.length(); i++) {
        char h = hex.charAt(i);
        if (h >= '0' && h <= '9')
            val |= h - '0';
        else
        if (h >= 'A' && h < 'F')
            val |= 10 + (h - 'A');
        else
        if (h >= 'a' && h < 'f')
            val |= 10 + (h - 'a');
        val <<= 4;
    }
    return val;
}

/* unit number of this panel driver, and its ascii hex char */
uint8_t unit = 0;
char unitc = '0';

bool ignoreLine = false;
String inbuff;

void setup() {
  /* these pins give the unit address */
  pinMode(PANELADDR_0, INPUT_PULLUP);
  pinMode(PANELADDR_1, INPUT_PULLUP);
  pinMode(PANELADDR_2, INPUT_PULLUP);
  pinMode(PANELADDR_3, INPUT_PULLUP);

  /* Serial port to receive commands */
  Serial.begin(115200);

  /* initialise the flipdot panel */
  display.begin();
  display.setRotation(0);
  display.clearDisplay();
  display.refresh();
  delay(200);

  unit = read_paneladdr();
  if (unit < 10)
      unitc = '0' + unit;
  else
      unitc = 'A' + (unit - 10);

  show_text("#" + (String)unit);
  delay(200);

  inbuff.reserve(32);
}

void processCommand(String incoming)
{
    char cmd = incoming.charAt(1);
    int x1, y1, x2, y2, c;

    switch (cmd) {
        case 'R':
            display.refresh();
            break;
        case 'C':
            display.display();
            break;
        case 'W':
            display.clearDisplay();
            break;
        case 'D':
            x1 = readHex(incoming.substring(2,4));
            y1 = readHex(incoming.substring(4,6));
            c = readHex(incoming.substring(6,7));
            display.drawPixel(x1, y1, c);
            break;
        case 'X':
            x1 = readHex(incoming.substring(2,4));
            y1 = readHex(incoming.substring(4,6));
            x2 = readHex(incoming.substring(6,8));
            y2 = readHex(incoming.substring(8,10));
            c = readHex(incoming.substring(10,11));
            if (c > 1)
                display.drawRect(x1, y1, x2, y2, c-2);
            else
                display.fillRect(x1, y1, x2, y2, c);
            break;
        case 'L':
            x1 = readHex(incoming.substring(2,4));
            y1 = readHex(incoming.substring(4,6));
            x2 = readHex(incoming.substring(6,8));
            y2 = readHex(incoming.substring(8,10));
            c = readHex(incoming.substring(10,11));
            display.drawLine(x1, y1, x2, y2, c);
            break;
        case 'S':
            x1 = incoming.charAt(2);
            c = readHex(incoming.substring(3,4));
            for (y1=0; y1<c; y1++) {
                switch (x1) {
                    case 'a': display.scrollleft(); break; 
                    case 'w': display.scrollup(); break; 
                    case 's': display.scrolldown(); break; 
                    case 'd': display.scrollright(); break; 
                }
            }
            break;
        case 'T':
            // FIXME
            show_text(incoming.substring(7));
            break;
        case 'B':
            // FIXME
            break;
        case 'M':
            // FIXME
            break;
    }
}

void loop() {
  /* if there is text available, read it */
  while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
          char target = inbuff.charAt(0);
          if (target == '*' || target == unitc)
              processCommand(inbuff);
          inbuff = "";
      } else
          inbuff += c;
  }
  /* any other regular tasks go here */
}
