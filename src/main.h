#ifndef MAIN_ADRIA
#define MAIN_ADRIA

// Enable disable debug with the following line.
// #define DEBUG

#ifdef DEBUG
#define PRINTLN(x) Serial.println(F(x));
#define PRINT(x) Serial.print(F(x));
#define PRINTF_1(x,y) Serial.printf(x,y);
#define PRINTF_2(x,y,z) Serial.printf(x,y,z);
#define PRINTF_4(x,y,z,a,b) Serial.printf(x,y,z,a,b);

#else
#define PRINTLN(x) 
#define PRINT(x)
#define PRINTF_1(x,y)
#define PRINTF_2(x,y,z)
#define PRINTF_4(x,y,z,a,b)
#endif

// Execute game main loop every x ms 
#define GAME_LOOP_MS 5

// Movement buttons pins
#define UP_BTN_PIN 21
#define DOWN_BTN_PIN 13

// Game over / win splash screen time in ms.
#define END_SCREEN_TIME 5000

#endif