/*

  ColdEND32 v1.0 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11

*/


// Board
#define COLDEND32                                     // Leave uncommented unless there are other boards to choose


// Display
#define SSD1306                                       // Uncomment for OLED with SSD1306 controller
// #define HT16K33                                      // Uncomment for 7-Segment LED Display with HT16K33 Controller
// #define LCD16X2                                      // Uncomment for 16x2 LCD or VFD with I2C Controller
// #define LCD16X4                                      // Uncomment for 16x4 LCD or VFD with I2C Controller


// Coolant pump
#define FLOW_DIR LOW                                  // Pump flow direction (LOW or HIGH)
#define MIN_RPM 0.5                                   // Minimum RPM of coolant pump
#define MAX_RPM 80                                    // Maximum RPM of coolant pump
#define FAST_RPM 80                                   // Fast mode RPM
#define SPIT_RPM 80                                   // Spit mode RPM
#define MAX_SPIT_TIME 8                               // Maximum spit time in seconds


// Operator control
#define MOMENTARY_SWITCH                              // Choose between normal switches and momentary switches
#define SCREEN_DELAY 100                              // Minimum delay between two screen refreshes in milliseconds
#define SWITCH_DEBOUNCE 30                            // Delay time in milliseconds to debounce momentary switches
#define REMOTE_CTRL                                   // Use switch input #4 for remote mist control (solid state signal). Momentary switches are required!
#define POT_FILTER 0.05                               // Noise filter for raw potentiometer values


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
  #define OUT_COOLANT 0                               // Coolant COOLANT valve pin
  #define OUT_LIFT 15                                 // Lift valve pin
  #define OUT_SPIT_LED 25                             // Spit LED pin
#endif


#ifdef SSD1306
  #define OLED
  #define OLED_ADD 0x3C                               // I2C address of OLED display
  #include <Adafruit_GFX.h>                           // Required library: https://github.com/adafruit/Adafruit-GFX-Library
  #include <Adafruit_SSD1306.h>                       // Required library: https://github.com/adafruit/Adafruit_SSD1306
  #include <Fonts/FreeSans18pt7b.h>
  Adafruit_SSD1306 oled(128, 64, &Wire, -1);
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
