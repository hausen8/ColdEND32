/*

  ColdEND32 v1.3 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2022-01-10

*/


// Board
#define COLDEND32                                     // Leave uncommented unless there are other boards to choose


// Display
// #define SSD1306                                       // Uncomment for OLED with SSD1306 controller
// #define SH1106                                        // Uncomment for OLED with SH1106 controller
// #define SSD1309                                       // Uncomment for OLED with SSD1309 controller
// #define HT16K33                                       // Uncomment for 7-Segment LED Display with HT16K33 Controller
// #define LCD16X2                                       // Uncomment for 16x2 LCD or VFD with I2C Controller
// #define LCD16X4                                       // Uncomment for 16x4 LCD or VFD with I2C Controller
// #define FIX16X4                                       // Uncomment to fix some 16x4 LCD row 3 and 4 starting at wrong positions


// Features
// #define DRAIN_SYSTEM                                  // Drain system with reversed fast mode when coolant is set to 0
// #define MOMENTARY_SWITCH                              // Uncomment for momentary switches instead of solid state switches
// #define REMOTE_CTRL                                   // Use input #4 for remote mist control from CNC. Momentary switches are required!
// #define EXT_LED                                       // Uncomment for external LEDs indicating active mode (mist, air or spit)


// Coolant pump
#define FLOW_DIR LOW                                  // Pump flow direction (LOW or HIGH)
#define MIN_RPM 0.5                                   // Minimum RPM of coolant pump
#define MAX_RPM 120                                   // Maximum RPM of coolant pump
#define FAST_RPM 80                                   // Fast mode RPM
#define SPIT_RPM 60                                   // Spit mode RPM
#define MIN_SPIT_TIME 0.1                             // Minimum spit time in seconds
#define MAX_SPIT_TIME 8                               // Maximum spit time in seconds (Values above 9 will overflow OLED!)


// Operator control
#define SWITCH_DEBOUNCE 30                            // Delay time in milliseconds to debounce momentary switches
#define REFRESH_TIME 100                              // Time between two screen refreshes in milliseconds
#define POT_FILTER 0.05                               // Noise filter for raw potentiometer values
#define POT_LOOP 1                                    // Loop time for reading pot values in ms


// Pinout
#ifdef COLDEND32
  #define POT_MIST 33                                 // Mist potentiometer pin
  #define POT_SPIT 32                                 // Spit potentiometer pin
  #define IN_MIST 36                                  // Mist switch
  #define IN_FAST 34                                  // Fast mode switch
  #define IN_AIR 39                                   // Air only switch
  #define IN_REM 35                                   // Remote control
  #define OUT_DIR 14                                  // Direction pin
  #define OUT_STEP 13                                 // Step pin
  #define OUT_ENABLE 12                               // Enable pin
  #define OUT_AIR 2                                   // Air valve pin
  #define OUT_COOLANT 0                               // Coolant valve pin
  #define OUT_LIFT 15                                 // Lift valve pin
  #define OUT_SPIT_LED 25                             // Spit LED pin
  #define OUT_5V_1 19                                 // 5V Output 1
  #define OUT_5V_2 18                                 // 5V Output 2
  #define OUT_5V_3 5                                  // 5V Output 3
  #define OUT_5V_4 4                                  // 5V Output 4
#endif


#if defined SSD1306 || defined SH1106 || defined SSD1309
  #define OLED
  #include <Arduino.h>
  #include <U8g2lib.h>                                // Required library: https://github.com/olikraus/U8g2_Arduino
  #include "Wire/Wire.h"
  #if defined SSD1306
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 
  #elif defined SH1106
    U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
  #else
    U8G2_SSD1309_128X64_NONAME0_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
  #endif
#endif


#ifdef HT16K33
  #define LED
  #define LED1_ADD 0x74                               // I2C address of first (mist) LED display (needs to be jumpered!)
  #define LED2_ADD 0x70                               // I2C address of second (spit) LED display (needs to be jumpered!)
  #define BRIGHTNESS 6                                // Brightness of LED displays
  #include <Wire.h>
  #include <Adafruit_LEDBackpack.h>                   // Required library: https://github.com/adafruit/Adafruit_LED_Backpack
  Adafruit_7segment led1 = Adafruit_7segment();
  Adafruit_7segment led2 = Adafruit_7segment();
#endif


#if defined LCD16X2 || defined LCD16X4
  #define LCD
  #define LCD_ADD 0x27                                // I2C address of LCD controller.
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>                      // Required library: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
  #ifdef LCD16X2
    LiquidCrystal_I2C lcd(LCD_ADD, 16, 2);
  #else
    LiquidCrystal_I2C lcd(LCD_ADD, 16, 4);
  #endif
#endif


#ifdef REMOTE_CTRL
  #define MOMENTARY_SWITCH                            // Remote control requires momentary switches
#endif


#ifdef MOMENTARY_SWITCH
  #include <Bounce2.h>                                // Required library: https://github.com/thomasfredericks/Bounce2
  Bounce btnMist = Bounce(IN_MIST, SWITCH_DEBOUNCE);
  Bounce btnAir = Bounce(IN_AIR, SWITCH_DEBOUNCE);
#endif





//
