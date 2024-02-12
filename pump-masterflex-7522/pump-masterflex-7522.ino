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
 *  
 *                     +-----------------------+
 *                     |    Wiring diagram     |
 *                     |          DB25         |
 *                     +-----------------------+
 *      _____________________________________________________
 *      [ 13  12  11  10  09  08  07  06  05  04  03  02  01 ]
 *      [                                                    ]
 *       \  25  24  23  22  21  20  19  18  17  16  15  14  / 
 *        \________________________________________________/
 *  
 *                     +-----------------------+  
 *                     |    Wiring diagram     |
 *                     |      Arduino UNO      |
 *                     +-----------------------+ 
 *                         _________________
 *                         |           SCL-|-
 *                         |           SDA-|-
 *                         |          AREF-|-
 *                         |           GND-|- 
 *                         |           D13-|- 
 *                        -|-IOREF     D12-|- 
 *                        -|-RESET     D11-|- 
 *                        -|-3.3V      D10-|- 
 *        mixer_relay_Vcc -|-5V        D09-|- 
 *                        -|-GND       D08-|-
 *                        -|-GND           | 
 *                        -|-VIN       D07-|- 
 *                         |           D06-|-  mixer_relay_in
 *                        -|-A0        D05-|- DB25_P01
 *                        -|-A1        D04-|- DB25_P15
 *                        -|-A2        D03-|- DB25_P16
 *               DB25_P14 -|-A3        D02-|- DB25_P20
 *                        -|-A4         TX-|-
 *                        -|-A5         RX-|-
 *                         |_______________|
 * 
 * 
 *                     +-----------------------+
 *                     | PINOUT & POWER SUPPLY |
 *                     |       & GROUND        |
 *                     +-----------------------+
 *                 
 *      COMPONENT        |             PIN             | ARDUINO PIN | POWER SUPPLY 
 *      -----------------+-----------------------------+-------------+--------------
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-01    |    D05      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-03    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-05    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-14    |    A3       |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-15    |    D04      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-16    |    D03      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-17    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-20    |    D02      |              
 *      MIXER            |          RELAY - GND        |    GND      |              
 *      MIXER            |          RELAY - VCC        |     5V      |          
 *      MIXER            |          RELAY - IN         |    D06      |              
*/

#include "PumpMasterflex.h"
#include "Controller.h"
#include "mixer.h"
#include <DFRobot_MAX31855.h>

#define PUMP_SPEED_CONTROL       5
#define PUMP_REMOTE_CONTROL      4
#define PUMP_CLOCKWISE_CONTROL   3
#define PUMP_PRIME_CONTROL       2
#define PUMP_SPEED_FEEDBACK      A3 // analog pin

#define MIXER_PIN                  6

/*----------------------------------------------------------------------------------------------------------*/


MasterflexDB25Interface_t mixing_pump_interface {
  .start_stop_pin  = PUMP_REMOTE_CONTROL,
  .direction_pin   = PUMP_CLOCKWISE_CONTROL,
  .prime_pin       = PUMP_PRIME_CONTROL,
  .voltage_in_pin  = PUMP_SPEED_CONTROL,
  .voltage_out_pin = PUMP_SPEED_FEEDBACK,
};

PumpMasterflex mixing_pump = PumpMasterflex(mixing_pump_interface);

mixer_t mixer;
DFRobot_MAX31855 max31855;

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  Serial.begin(9600);
  mixer.pin = MIXER_PIN;
  pinMode(mixer.pin,OUTPUT);
  digitalWrite(mixer.pin, LOW);
  mixing_pump.Connect();
  mixing_pump.Stop();
  mixing_pump.PrimeStop();
  mixing_pump.SetDirection(DIR_CW);
  mixing_pump.SetTubeSize(14);
  mixing_pump.SetMaxVoltageLevel(5000);
  mixing_pump.SetMinVoltageLevel(100);
  mixing_pump.SetMinSpeed(0);
  mixing_pump.SetMaxSpeed(37700);
  mixing_pump.SetSpeed(14000);
  mixing_pump.PipeSetVol(1600);
  mixing_pump.PipeSetVol(1700);
  // temp sensor
  max31855.begin();
  Serial.println("READY");
}

int32_t res;
cmd_t command;
resp_t response;
bool readTemp = false;
long time_sec = 0;
/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  // Serial.println("Ready");
  if (GetCommand(&command) == CMD_RECEIVED)
  {
    if (command.target.equals("FIRE"))
    {
      if (command.command_id.equals("READ"))
      {
        readTemp = true;
        time_sec = command.operand;
        res = CMD_RECEIVED;
      }
      else {
        res = CMD_INVALID;
      }
    }
    if (command.target.equals("LIGHTER"))
    {
      digitalWrite(3, HIGH);
      delay(command.operand);
      digitalWrite(3, LOW);
      res = CMD_RECEIVED;
    }
    else if (command.target.equals(MIXER))
    {
      res = MixerController(&mixer, &command);
    }
    else if (command.target.equals(MIXINGPUMP))
    {
      res = PumpController(&mixing_pump, &command);
    }
    else {
      res = CMD_INVALID;
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
  if (readTemp == true)
  {
    TempSensorRead(time_sec);
    readTemp = false;
  }
}

void TempSensorRead(long time_sec)
{
  float temp;
  int IRsensor_data;
  while (--time_sec > 0)
  {
    temp = max31855.readCelsius();
    IRsensor_data = digitalRead(12);
    Serial.print("IR Sensor: ");
    if (IRsensor_data == LOW)
    {
      Serial.println("Fire detected");
    }
    else {
      Serial.println("No fire");
    }
    Serial.print("Temp Sensor: ");
    Serial.print(temp);
    Serial.println(" deg C");
    delay(500);
  }
}