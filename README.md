# ColdEND32
Firmware for ColdEND32 controller supporting various displays

Installation and configuration:

1) Download and import the following libraries depending on your display:

   **SSD1306 or SH1106**
   https://github.com/olikraus/U8g2_Arduino

   **HT16K33**
   https://github.com/adafruit/Adafruit_LED_Backpack

   **LCD 16x2 or 16x4**
   https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

2) Uncomment (define) the appropriate display in config.h. Note that only one type of
   display at one time is allowed so dont' forget to comment the other ones.
   For 7 segment LED displays, don't forget to jumper different I2C addresses for both
   displays and enter these values in config.h ("LED1_ADD" and "LED2_ADD").

3) Uncomment (define) MOMENTARY_SWITCH if you want to use monentary switches.
   Required library: https://github.com/thomasfredericks/Bounce2
   For normal switches leave "momentary_switch" commented.

4) Uncomment (define) DRAIN_SYSTEM if you want to have the option to drain the coolant
   pump and pipes anytime. To drain your system, turn the coolant pot all the way down
   and push the fast mode switch.

5) Uncomment (define) REMOTE_CTRL if you want to control your ColdEND32 system from the
   CNC controller with a 24V solid signal (must be connected to input #4).

6) Flash your ColdEND controller and do a first check:

   Problem                                         | Solution
   ------------------------------------------------|-----------
   Pump speed range does not match my requirements | Edit MIN_RPM and/or MAX_RPM
   Flow direction of pump is wrong                 | Edit FLOW_DIR
   Fast mode speed is too slow/fast                | Edit FAST_RPM
   Spit mode speed is too slow/fast                | Edit SPIT_RPM
   Spit time range dows not match my requirements  | Edit MIN_SPIT_TIME and/or MAX_SPIT_TIME
   Spit mode does not start at times               | Slightly increase SWITCH_DEBOUNCE
   Momentary switches do not work properly         | Slightly increase SWITCH_DEBOUNCE
   Display seems to be too slow                    | Decrease REFRESH_TIME
   Mist or Spit pot values are jumping             | Try lower POT_FILTER values
   Mist or Spit pot values are still jumping       | Increase POT_LOOP to 2 or 3

7) If any of these values have been altered, you need to flash the controller once again.


**HISTORY:**

- v1.0: Initial firmware for ESP32 controlled pump board
