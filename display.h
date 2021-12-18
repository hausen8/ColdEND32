/*

  ColdEND32 v1.0 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11

*/


void refDisplay() {
  // refresh display
  #ifdef OLED
    if (mist_pot_val < 10) {
      dtostrf(mist_pot_val, 3, 1, disp_mist);         // Conversion from float to string
      cursor_pos = 51;                                    // Set cursor depending on string length
    }
    else if (mist_pot_val < 100) {
      dtostrf(mist_pot_val, 2, 0, disp_mist);
      cursor_pos = 41;
    }
    else {
      dtostrf(mist_pot_val, 3, 0, disp_mist);
      cursor_pos = 60;
    }
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.print("Coolant");
    oled.setCursor(101, 0);
    oled.print("Spit");
    oled.drawLine(0, 12, 128, 12, WHITE);
    oled.setFont(&FreeSans18pt7b);
    oled.setCursor(0, 42);
    oled.print(disp_mist);
    oled.setFont();
    oled.setTextSize(1);
    oled.setCursor(cursor_pos, 36);
    oled.print("rpm");
    oled.setFont(&FreeSans18pt7b);
    oled.setCursor(101, 42);
    oled.print(spit_pot_val, 0);
    oled.setFont();
    oled.setTextSize(1);
    oled.setCursor(122, 36);
    oled.print("s");
    oled.drawLine(0, 48, 128, 48, WHITE);
    oled.setTextColor(BLACK);
    
    if (spit_mode == 1) {
      oled.fillRect(0, 54, 57, 10, WHITE);
      oled.setCursor(2, 55);
      oled.print("Spit Mode");
      // drawPump(iconvalue, 1);
    }
    else if (coolant_valve == true) {
      oled.fillRect(0, 54, 63, 10, WHITE);                // Draw white rectangle with black text
      oled.setCursor(2, 55);
      oled.print("Coolant On");
      // drawPump(iconvalue, 1);
    }
    if (air_valve == true) {
      oled.fillRect(89, 54, 39, 10, WHITE);
      oled.setCursor(91, 55);
      oled.print("Air On");
    }
    oled.display();
  #endif


  #ifdef LED
    led1.print(mist_pot_val);
    led1.writeDisplay();
    led2.clear();
    if (coolant_valve == true) {
      led2.writeDigitRaw(0, 0b10111001);                // Draw "C"
    }
    if (air_valve == true) {
      led2.writeDigitRaw(1, 0b11110111);                // Draw "A"
    }
    led2.writeDigitNum(4, spit_pot_val);
    led2.writeDisplay();
  #endif


  #ifdef LCD
    if (mist_pot_val != mist_pot_old || spit_pot_val != spit_pot_old) {
      if (mist_pot_val < 10) {
        digits = 1;
      }
      else {
        digits = 0;
      }
      lcd.clear();
      lcd.print("Coolant: ");
      lcd.print(mist_pot_val, digits);
      lcd.print("rpm");
      lcd.setCursor(0, 1);
      lcd.print("Spit T.: ");
      lcd.print(spit_pot_val, 1);
      lcd.print("s");
      #ifdef LCD16X4
        lcd.setCursor(-4, 2);                             // For some reason cursor position at 3rd and 4rth line
        lcd.print("C. Valve: ");                          // start with -4 instead of 0
        lcd.setCursor(10, 2);
        if (coolant_valve == true) {
          lcd.print("On");
        }
        else {
          lcd.print("--");
        }
        lcd.setCursor(-4, 3);
        lcd.print("A. Valve: ");
        lcd.setCursor(10, 3);
        if (air_valve == true) {
          lcd.print("On");
        }
        else {
          lcd.print("--");
        }
      #endif
    }
  #endif
}


void prepDisplay() {
  // Call screen refresh function by defined delay
  curr_refresh = millis();
  if (curr_refresh - prev_refresh >= SCREEN_DELAY) {
    refDisplay();
    prev_refresh = curr_refresh;
  }
}





//
