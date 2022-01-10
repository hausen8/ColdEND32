/*

  ColdEND32 v1.3 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2022-01-10

*/


void dispContent() {
  #ifdef OLED
    if (mist_val < 10) {
      digits = 1;                                       // If value < 10, display as float with one decimal place
      cursor_pos = 50;                                  // Set cursor depending on string length
    }
    else if (mist_val < 100) {
      digits = 0;                                       // Display rounded to int
      cursor_pos = 41;
    }
    else {
      digits = 0;
      cursor_pos = 60;
    }
    u8g2.clearBuffer();
    u8g2.setFontMode(0);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_lucasfont_alternate_tr);
    u8g2.drawStr(0, 7, "Coolant");
    u8g2.drawStr(101, 7, "Spit");
    u8g2.drawLine(0, 12, 128, 12);
    u8g2.setFont(u8g2_font_fur25_tf);
    u8g2.setCursor(0, 43);
    u8g2.print(mist_val, digits);
    u8g2.setFont(u8g2_font_lucasfont_alternate_tr);
    u8g2.drawStr(cursor_pos, 43, "rpm");
    u8g2.setFont(u8g2_font_fur25_tf);
    u8g2.setCursor(100, 43);
    u8g2.print(spit_val, 0);
    u8g2.setFont(u8g2_font_lucasfont_alternate_tr);
    u8g2.drawStr(122, 43, "s");
    u8g2.drawLine(0, 48, 128, 48);
    if (spit_mode == true) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 53, 55, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(3, 62, "Spit Mode");
    }
    else if (coolant_valve == true) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(0, 53, 60, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(3, 62, "Coolant On");
    }
    if (air_valve == true) {
      u8g2.setDrawColor(1);
      u8g2.drawBox(93, 53, 39, 11);
      u8g2.setDrawColor(0);
      u8g2.drawStr(96, 62, "Air On");
    }
    u8g2.sendBuffer();
  #endif


  #ifdef LED
    led1.print(mist_val);
    led1.writeDisplay();
    led2.clear();
    if (coolant_valve == true) {
      led2.writeDigitRaw(0, 0b10111001);                // Draw "C"
    }
    if (air_valve == true) {
      led2.writeDigitRaw(1, 0b11110111);                // Draw "A"
    }
    led2.writeDigitNum(4, spit_val);
    led2.writeDisplay();
  #endif


  #ifdef LCD
    #ifdef FIX16X4
      start_pos = -4;                                   // Fix some 16x4 LCD starting row 3 and 4 at wrong positions
    #endif
    
    if (mist_val < 10) {
      digits = 1;                                       // If value < 10, display as float with one decimal place
      cursor_pos = 10;
    }
    else if (mist_val < 100) {
      digits = 0;                                       // Display rounded to int
      cursor_pos = 11;
    }
    else {
      digits = 0;                                       // Display rounded to int
      cursor_pos = 10;
    }
    lcd.clear();
    lcd.print("Coolant:");
    lcd.setCursor(cursor_pos, 0);
    lcd.print(mist_val, digits);
    lcd.print("rpm");
    lcd.setCursor(0, 1);
    lcd.print("Spit T.:");
    lcd.setCursor(14, 1);
    lcd.print(spit_val, 0);
    lcd.print("s");
    #ifdef LCD16X4
      lcd.setCursor(start_pos, 2);
      lcd.print("C. Valve:");
      lcd.setCursor(start_pos+14, 2);
      if (coolant_valve == true) {
        lcd.print("On");
      }
      else {
        lcd.print("--");
      }
      lcd.setCursor(start_pos, 3);
      lcd.print("A. Valve:");
      lcd.setCursor(start_pos+14, 3);
      if (air_valve == true) {
        lcd.print("On");
      }
      else {
        lcd.print("--");
      }
    #endif
  #endif
}


void refDisplay() {
  curr_refresh = millis();
  if (curr_refresh - prev_refresh >= REFRESH_TIME) {
    if (spit_mode == true) {
      spit_sub += (REFRESH_TIME / 1000.0);              // Calculate time to subtract from spit time during spit mode
      spit_val -= spit_sub;                             // Spit time countdown
      if (spit_val < 0) {
        spit_val = 0;
      }
    }
    else {
      spit_sub = 0;                                     // Reset subtraction
    }
    if (mist_val != mist_old || spit_val != spit_old || coolant_valve != coolant_old || air_valve != air_old) {
      dispContent();                                    // Refresh display content only when something has changed

      mist_old = mist_val;                              // Save values to compare next time
      spit_old = spit_val;
      coolant_old = coolant_valve;
      air_old = air_valve;
    }
    prev_refresh = curr_refresh;
  }
}





//
