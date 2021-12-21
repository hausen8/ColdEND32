# ColdEND32
Firmware for ColdEND32 controller supporting various displays

Installation and configuration:

1) Download and import the following libraries depending on your displays:

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

3) Uncomment (define) "momentary_switch" if you want to use monentary switches.
   Required library: https://github.com/thomasfredericks/Bounce2
   For normal switches leave "momentary_switch" commented.

4) Uncomment (define) "drain_system" if you want to have the option to drain the coolant
   pump and pipes anytime. For draining the system, set the coolant to 0 ml/h and push the
   fast mode switch.

5) Flash your ColdEND controller and check if MIN_RPM and MAX_RPM match the desired range.
   Check spit mode and fast mode. If any of these values need to get changed, you have to
   flash the controller once again.

6) If spit mode at times does not start or any of your momentary switches seem to work not properly,
   it is most likely that they are bouncing. In this case, increase SWITCH_DEBOUNCE slightly.


**HISTORY:**

- v1.0: Initial firmware for ESP32 controlled pump board
