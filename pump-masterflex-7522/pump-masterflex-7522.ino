/*
 * PUMP CONTROL MASTERFLEX 7522
 * Arduino UNO to DB25
 * DB25 pinout:
 * DB25_P1 : Speed control INPUT voltage 0-10V
 * DB25_P3 : Speed control INPUT GROUND 
 * DB25_P5 : Speed signal OUTPUT GROUND
 * DB25_P14: Speed signal OUTPUT voltage 0-10V
 * DB25_P15: Remote START/STOP INPUT
 * DB25_P16: Remote CW/CCW INPUT
 * DB25_P17: Remote START/STOP, CW/CCW, Prime GROUND
 * DB25_P20: Remote prime INPUT
 *                 +-----------------------+
 *                 |    Wiring diagram     |
 *                 |          DB25         |
 *                 +-----------------------+
 *   _____________________________________________________
 *   [ 13  12  11  10  09  08  07  06  05  04  03  02  01 ]
 *   [                                                    ]
 *    \  25  24  23  22  21  20  19  18  17  16  15  14  / 
 *     \________________________________________________/
 * 
 *                 +-----------------------+  
 *                 |    Wiring diagram     |
 *                 |      Arduino UNO      |
 *                 +-----------------------+ 
 *                     _________________
 *                     |           SCL-|-
 *                     |           SDA-|-
 *                     |          AREF-|-
 *                     |           GND-|- DB25_P3
 *                     |           D13-|- 
 *                    -|-IOREF     D12-|- 
 *                    -|-RESET     D11-|- DB25_P1 
 *                    -|-3.3V      D10-|- DB25_P15
 *                    -|-5V        D09-|- DB25_P16
 *           DB25_P17 -|-GND       D08-|- DB25_P20
 *            DB25_P5 -|-GND           | 
 *                    -|-VIN       D07-|-
 *                     |           D06-|-
 *           DB25_P14 -|-A0        D05-|-
 *                    -|-A1        D04-|-
 *                    -|-V2        D03-|-
 *                    -|-A3        D02-|-
 *                    -|-A4         TX-|-
 *                    -|-A5         RX-|-
 *                     |_______________|
 * 
 */

#include "PumpMasterflex.h"

// Define pinout
#define SPEED_CONTROL       11
#define REMOTE_CONTROL      10
#define CLOCKWISE_CONTROL   9
#define PRIME_CONTROL       8
#define SPEED_FEEDBACK      A0 // analog pin

/*----------------------------------------------------------------------------------------------------------*/

MasterflexDB25Interface_t pump_interface {
  .start_stop_pin  = REMOTE_CONTROL,
  .direction_pin   = CLOCKWISE_CONTROL,
  .prime_pin       = PRIME_CONTROL,
  .voltage_in_pin  = SPEED_CONTROL,
  .voltage_out_pin = SPEED_FEEDBACK,
};

PumpMasterflex pump = PumpMasterflex(pump_interface);

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  Serial.begin(9600);
  pump.Connect();
  pump.Stop();
  pump.PrimeStop();
  pump.SetDirection(DIR_CW);
}

/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  pump.Start();
  delay(1000);
}