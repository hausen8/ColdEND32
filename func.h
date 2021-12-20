/*

  ColdEND32 v1.0 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2021-12-20

*/


void switchStat() {
  // Read fast mode momentary switch
  fast_mode = digitalRead(IN_FAST);
  
  // Either read momentary switches or solid state switches
  #ifdef MOMENTARY_SWITCH
  
    // Read remote mist control
    // There is no priority between remote control and momentary switches
    // All signals can be overwritten by each other
    #ifdef REMOTE_CTRL
      rem_stat = digitalRead(IN_REM);
      if (rem_stat != rem_stat_old) {
        mist_stat = rem_stat;                     // Switch mist stat
        air_stat = LOW;                           // switch off air stat
        rem_stat_old = rem_stat;
      }
    #endif
    
    // Read momentary mist switch
    btnMist.update();
    if(btnMist.fell()) {
      mist_stat = !mist_stat;                     // Toggle between mist on and off
      air_stat = LOW;                             // switch off air stat
    }
    
    // Read momentary air switch
    btnAir.update();
    if(btnAir.fell()) {
      air_stat = !air_stat;                       // Toggle between air on and off
      mist_stat = LOW;                            // switch off mist stat
    }
  #else
    mist_stat = digitalRead(IN_MIST);
    air_stat = digitalRead(IN_AIR);
  #endif
}


void setValves() {
  // Switch control relays for solenoid valves
  if (fast_mode == HIGH || mist_stat == HIGH) {
    digitalWrite(OUT_COOLANT, HIGH);
    coolant_valve = true;
  }
  else {
    digitalWrite(OUT_COOLANT, LOW);
    coolant_valve = false;
  }
  if (mist_stat == HIGH || air_stat == HIGH) {
    digitalWrite(OUT_AIR, HIGH);
    air_valve = true;
  }
  else {
    digitalWrite(OUT_AIR, LOW);
    air_valve = false;
  }
}


void potVals() {
  curr_pot_read = millis();
  if (curr_pot_read - prev_pot_read >= POT_LOOP) {
    mist_pot_old = mist_pot_val;
    float mist_pot_raw = exp(log(MIN_RPM) + analogRead(POT_MIST)*exp_scale);            // Map mist pot range to exponential RPM range (MIN_RPM to MAX_RPM)
    mist_pot_val = POT_FILTER*mist_pot_raw + (1-POT_FILTER)*mist_pot_old;               // Denoise value with exponential filter
    mist_val = round(mist_pot_val*10)/10;
    
    spit_pot_old = spit_pot_val;
    float spit_pot_raw = (analogRead(POT_SPIT)*max_spit)/4095;                          // Map spit pot range to spit time (0 to MAX_SPIT_TIME)
    spit_pot_val = POT_FILTER*spit_pot_raw + (1-POT_FILTER)*spit_pot_old;               // Denoise value with exponential filter
    spit_val = round(spit_pot_val*10)/10;
    
    prev_pot_read = curr_pot_read;
  }
}


void spitLED() {
  if (spit_pot_val >= MIN_SPIT_TIME) {
    digitalWrite(OUT_SPIT_LED, HIGH);
  }
  else {
    digitalWrite(OUT_SPIT_LED, LOW);
  }
}


void pumpControl() {
  if (coolant_valve == true) {
    #ifdef DRAIN_SYSTEM
      if (fast_mode == HIGH && mist_pot_val <= MIN_RPM+0.01) {
        digitalWrite(OUT_DIR, !FLOW_DIR);                       // Reverse flow direction in fast mode when mist is turned all the way down to drain the system
      }
      else {
        digitalWrite(OUT_DIR, FLOW_DIR);
      }
    #endif
    
    if (spit_pot_val >= MIN_SPIT_TIME && mist_stat == HIGH && spit_stat == false) {
      int spit_time = spit_pot_val*1000000;                     // Get spit time from pot (convert s to µs)
      timerAlarmWrite(spitTimer, spit_time, true);              // Set timer alarm to spit time, autoreload = true (since we need the 2nd interrupt)
      timerAlarmEnable(spitTimer);
      spit_mode = true;
    }
    else if (spit_mode == true) {
      timerAlarmDisable(spitTimer);                             // Stop spit timer
      spit_mode = false;
    }

    if (spit_mode == true) {
      rpm = SPIT_RPM;                                           // Select RPM source
    }
    else if (fast_mode == HIGH) {
      rpm = FAST_RPM;
    }
    else {
      rpm = mist_pot_val;
    }
    digitalWrite(OUT_ENABLE, LOW);                              // Enable stepper driver
    int edge = round(1000000/(rpm/60*6400*2));                  // Pulse time calculation for stepper to get the time in µs for a rising or falling edge
    timerAlarmWrite(stepTimer, edge, true);                     // Set timer alarm to pulse length/2, autoreload = true
    timerAlarmEnable(stepTimer);
  }
  else {
    timerAlarmDisable(stepTimer);                               // Stop timers and reset all states
    timerAlarmDisable(spitTimer);
    digitalWrite(OUT_ENABLE, HIGH);
    spit_stat = false;
    spit_mode = false;
    spit_stop = false;
  }
}


void IRAM_ATTR spitMode(){
  portENTER_CRITICAL_ISR(&timerMux1);
  if (spit_stop == true) {
    spit_stat = true;
  }
  spit_stop = true;
  portEXIT_CRITICAL_ISR(&timerMux1);
}


void IRAM_ATTR stepPulse(){
  // Step pulse generated by hardware timer
  portENTER_CRITICAL_ISR(&timerMux0);
  pulse = !pulse;
  digitalWrite(OUT_STEP, pulse);
  portEXIT_CRITICAL_ISR(&timerMux0);
}





//
