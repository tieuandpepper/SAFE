/**
 * @file arduino-main.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "src/PumpMasterflex.h"
#include "Controller.h"
#include "src/mixer.h"
#include <DFRobot_MAX31855.h>
#include "arduino_pinout.h"
#include "src/rotary_valve.h"
/*----------------------------------------------------------------------------------------------------------*/


MasterflexDB25Interface_t mixing_pump_interface {
  .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
  .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
  .prime_pin       = PIN_MIXING_PUMP_PRIME_CONTROL,
  .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
  .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
};

PumpMasterflex mixing_pump = PumpMasterflex(mixing_pump_interface);
RotaryValve rotary_valve(9,10, 16, 9600);
mixer_t mixer;
DFRobot_MAX31855 max31855;

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  Serial.begin(9600);
  mixer.pin = PIN_MIXER_ENABLE;
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
  mixing_pump.SetSpeed(25000);
  // mixing_pump.PipeSetVol(1600);
  mixing_pump.PipeSetVol(2000);
  // temp sensor
  max31855.begin();
  pinMode(7, OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  delay(200);
  digitalWrite(6,LOW);
  rotary_valve.initialize();
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
    else if (command.target.equals("LIGHTER"))
    {
      digitalWrite(7, HIGH);
      delay(command.operand);
      digitalWrite(7, LOW);
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
    else if (command.target.equals("ROTVALVE"))
    {
      if (command.command_id.equals("PORT"))
      {
        res = rotary_valve.ActionMoveAuto((uint8_t)command.operand);
      }
      else if (command.command_id.equals("GETVER"))
      {
        uint32_t version;
        res = rotary_valve.QueryCurrVersion(&version);
        if (res == RESP_DRV_SUCCESS)
        {
          res = version;
        }
      }
      else {
        res = CMD_INVALID;
      }
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
  if (readTemp  && (time_sec-- > 0))
  {
    TempSensorRead();
  }
  if (time_sec == 0)
  {
    readTemp = false;
  }
  delay(200);
}

void TempSensorRead()
{
  float temp;
  int IRsensor_data;
  temp = max31855.readCelsius();
  // IRsensor_data = digitalRead(12);
  // Serial.print("IR Sensor: ");
  // if (IRsensor_data == LOW)
  // {
  //   Serial.println("Fire detected");
  // }
  // else {
  //   Serial.println("No fire");
  // }
  Serial.print("Temp Sensor: ");
  Serial.print(temp);
  Serial.println(" deg C");
}