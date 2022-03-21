#include "screen.h"

Screen::Screen(uint8_t w, uint8_t h, TwoWire *twi,
               int8_t rst_pin, uint32_t clkDuring,
               uint32_t clkAfter) : Adafruit_SSD1306(w, h, twi, rst_pin, clkDuring, clkAfter) {}

void Screen::init(void)
{
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    PRINTLN(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
}

void Screen::text(const char *text, uint8_t x, uint8_t y, uint8_t size)
{
  setTextSize(size);
  setTextColor(WHITE);
  setCursor(x, y);
  print(text);   
}

void Screen::centerText(const char *text, uint8_t x, uint8_t y, uint8_t size)
{
  setTextSize(size);
  setTextColor(WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  setTextWrap(false);
  getTextBounds(text, x, y, &x1, &y1, &w, &h);
  setTextWrap(true);
  setCursor(x - w / 2,y);
  print(text);  
}

void Screen::centerCursor(const char *text, uint8_t x, uint8_t y, uint8_t size)
{
  setTextSize(size);
  setTextColor(WHITE);
  int16_t x1, y1;
  uint16_t w, h;
  setTextWrap(false);
  getTextBounds(text, x, y, &x1, &y1, &w, &h);
  setTextWrap(true);
  setCursor(x - w / 2, y);
}