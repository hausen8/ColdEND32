/*

  ColdEND32 v1.2 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2021-12-29

*/


// Potentiometers
float           mist_pot_val = 0.00;                    // Mist value in milliliter per minute
float           spit_pot_val = 0.00;                    // Spit value in seconds
float           mist_pot_old = 0.00;                    // Previous mist pot value for exponential smoothing
float           spit_pot_old = 0.00;                    // Previous spit pot value for exponential smoothing
unsigned long   prev_pot_read = 0;                      // Previous pot read loop
unsigned long   curr_pot_read = 0;                      // Current pot read loop
float           max_spit = MAX_SPIT_TIME;               // Convert MAX_SPIT_TIME to float
float           exp_scale = log(MAX_RPM/MIN_RPM)/4095;  // Exponential flow scaling to match a 12bit analogue value


// Relais states
bool            coolant_valve = false;                  // Coolant valve state (on/off)
bool            air_valve = false;                      // Air valve state (on/off)


// Switch states
bool            spit_mode = false;                      // Spit mode state
bool            spit_stat = false;                      // Flag indicating whether spit mode has been executed or not
byte            fast_mode = LOW;                        // Fast mode state
byte            mist_stat = LOW;                        // Mist switch state
byte            air_stat = LOW;                         // Air switch state
byte            rem_stat = LOW;                         // Remote mist control state
byte            rem_stat_old = LOW;                     // Last remote state


// Stepper
volatile byte   pulse = LOW;                            // Step pulse state
float           rpm = 0.00;                             // Stepper RPM
unsigned long   spit_start = 0;                         // Timestamp for spit start


// Display
unsigned long   prev_refresh = 0;                       // Previous display refresh time
unsigned long   curr_refresh = 0;                       // Current display refresh time
float           mist_val = 0.0;                         // Displayed mist value
int             spit_val = 0;                           // Displayed spit value
float           spit_sub = 0.0;                         // Subtracted from spit time during spit mode countdown
int             start_pos = 0;                          // Variable start position to fix some 16x4 LCD
int             cursor_pos = 0;                         // Variable cursor position
int             digits = 0;                             // Mist value decimal places
float           mist_old = 0.0;                         // Values from previous refresh
int             spit_old = 0;
byte            coolant_old = LOW;
byte            air_old = LOW;






//
