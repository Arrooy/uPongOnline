#ifndef _OLED_H_
#define _OLED_H_

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "main.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET 16 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_SDA 4
#define OLED_SCL 15

#define SSD1306_NO_SPLASH // Eliminar splash screen
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

class Screen : public Adafruit_SSD1306{
    public:
    Screen(uint8_t w, uint8_t h, TwoWire *twi = &Wire,
                   int8_t rst_pin = -1, uint32_t clkDuring = 400000UL,
                   uint32_t clkAfter = 100000UL);
    ~Screen(){};
    
    void init(void);
    
    void text(const char *text, uint8_t x, uint8_t y, uint8_t size);
    
    void centerText(const char * text, uint8_t x, uint8_t y, uint8_t size);
    void centerCursor(const char *text, uint8_t x, uint8_t y, uint8_t size);
};
#endif