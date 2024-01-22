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
 *                     |           GND-|- 
 *                     |           D13-|- mixer_relay_in
 *                    -|-IOREF     D12-|- 
 *                    -|-RESET     D11-|- DB25_P1 - 1
 *                    -|-3.3V      D10-|- DB25_P15 - 1
 *    mixer_relay_Vcc -|-5V        D09-|- DB25_P16 - 1
 *                    -|-GND       D08-|- DB25_P20 - 1
 *                    -|-GND           | 
 *                    -|-VIN       D07-|- DB25_P1 - 2
 *                     |           D06-|- DB25_P15 - 2
 *       DB25_P14 - 1 -|-A0        D05-|- DB25_P16 - 2
 *       DB25_P14 - 2 -|-A1        D04-|- DB25_P20 - 2
 *                    -|-A2        D03-|-
 *                    -|-A3        D02-|-
 *                    -|-A4         TX-|-
 *                    -|-A5         RX-|-
 *                     |_______________|
 * 
 * GND: DB25_P17 (1,2), DB25_P3(1,2), DB25_P5(1,2), mixer_relay_GND
 */

#include "PumpMasterflex.h"
#include "Controller.h"
#include "mixer.h"
#include <DFRobot_MAX31855.h>

#define PUMP_COUNT                  2
// Define pinout
#define PUMP_2_SPEED_CONTROL       11
#define PUMP_2_REMOTE_CONTROL      10
#define PUMP_2_CLOCKWISE_CONTROL   9
#define PUMP_2_PRIME_CONTROL       8
#define PUMP_2_SPEED_FEEDBACK      A0 // analog pin

#define PUMP_1_SPEED_CONTROL       7
#define PUMP_1_REMOTE_CONTROL      6
#define PUMP_1_CLOCKWISE_CONTROL   5
#define PUMP_1_PRIME_CONTROL       4
#define PUMP_1_SPEED_FEEDBACK      A1 // analog pin

#define MIXER_PIN                      13

/*----------------------------------------------------------------------------------------------------------*/


MasterflexDB25Interface_t pump_1_interface {
  .start_stop_pin  = PUMP_1_REMOTE_CONTROL,
  .direction_pin   = PUMP_1_CLOCKWISE_CONTROL,
  .prime_pin       = PUMP_1_PRIME_CONTROL,
  .voltage_in_pin  = PUMP_1_SPEED_CONTROL,
  .voltage_out_pin = PUMP_1_SPEED_FEEDBACK,
};

MasterflexDB25Interface_t pump_2_interface {
  .start_stop_pin  = PUMP_2_REMOTE_CONTROL,
  .direction_pin   = PUMP_2_CLOCKWISE_CONTROL,
  .prime_pin       = PUMP_2_PRIME_CONTROL,
  .voltage_in_pin  = PUMP_2_SPEED_CONTROL,
  .voltage_out_pin = PUMP_2_SPEED_FEEDBACK,
};

PumpMasterflex pump[] = 
{
  PumpMasterflex(pump_1_interface),
  PumpMasterflex(pump_2_interface),
};

mixer_t mixer;
DFRobot_MAX31855 max31855;

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  Serial.begin(9600);
  mixer.pin = MIXER_PIN;
  pinMode(mixer.pin,OUTPUT);
  digitalWrite(mixer.pin, LOW);
  for (int i = 0; i < PUMP_COUNT; i++)
  {
    pump[i].Connect();
    pump[i].Stop();
    pump[i].PrimeStop();
    pump[i].SetDirection(DIR_CW);
    pump[i].SetTubeSize(14);
    pump[i].SetMaxVoltageLevel(5000);
    pump[i].SetMinVoltageLevel(100);
    pump[i].SetMinSpeed(0);
    pump[i].SetMaxSpeed(37700);
    pump[i].SetSpeed(14000);
  }
  pump[0].PipeSetVol(1600);
  pump[1].PipeSetVol(1700);
  max31855.begin();
  Serial.println("READY");
}

int32_t res;
cmd_t command;
resp_t response;
/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  // Serial.println("Ready");
  if (GetCommand(&command) == CMD_RECEIVED)
  {
    if (command.target.equals("TEMP"))
    {
      float temp = max31855.readCelsius();
      res = (int32_t)temp;
    }
    else if (command.target.equals(MIXER))
    {
      res = MixerController(&mixer, &command);
    }
    else
    {
      res = PumpController(pump, &command);
    }
    response.source = command.target;
    // Serial.println("READY");
    if (res == CMD_INVALID)
    {
      response.resp_id = RESP_INVALID;
      // Serial.println("Invalid command");
    }
    else 
    {
      // Serial.print("Command return ");
      // Serial.println(res);
      response.resp_id = RESP_VALID;
      response.data = res;
    }
    SendResponse(response);
    command.operand = 0;
    response.data = 0;
  }
  delay(200);
}
