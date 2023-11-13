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
 * 
 */

#include "PumpMasterflex.h"
// #include "Comm.h"

// Define pinout
#define SPEED_CONTROL       11
#define REMOTE_CONTROL      10
#define CLOCKWISE_CONTROL   9
#define PRIME_CONTROL       8
#define SPEED_FEEDBACK      A0 // analog pin

#define BUFFER_SIZE         40

/*----------------------------------------------------------------------------------------------------------*/
bool new_data = false;
char buffer[BUFFER_SIZE];
uint16_t msg_idx = 0;
bool data_received = false;

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
  pump.SetTubeSize(14);
  pump.SetMaxVoltageLevel(5000);
  pump.SetMinVoltageLevel(400);
  pump.SetMinSpeed(0);
  pump.SetMaxSpeed(30000);
//  pump.SetSpeed(6820);
  Serial.println("Initialization completed!");
  Serial.println(pump.GetSpeed());
}

/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  ReadSerial();
  Controller();
}

void ReadSerial()
{
  char c;
//  msg_idx = 0;
  while (Serial.available() > 0)
  {
    c = Serial.read();
    if (c != '\n' && (msg_idx < BUFFER_SIZE - 1))
    {
      buffer[msg_idx++] = c;
    }
    else
    {
      buffer[msg_idx] = '\0';
      data_received = true;
      msg_idx = 0;
      Serial.print("Received data: ");
      Serial.println(buffer);
    }
  }
}

void Controller()
{
  if (!data_received)
  {
    return;
  }
  switch (buffer[0])
  {
  case '1':
    Serial.println("start");
    pump.Start();
    break;
  case '2':
    Serial.println("stop");
    pump.Stop();
    break;
  case '3':
    Serial.println("switch");
    pump.ChangeDirection();
    break;
  case '4':
    Serial.println("dispense. Continuous mode");
    long int t1 = millis();
    pump.Dispense(3000);
    t1 = millis() - t1;
    Serial.print("done in ");
    Serial.println(t1);
    break;
  default:
    break;
  }
  data_received = false;
}
