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

#include "src/device_controller.h"
#include "arduino_pinout.h"
/*----------------------------------------------------------------------------------------------------------*/


MasterflexDB25Interface_t mixing_pump_interface {
  .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
  .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
  .prime_pin       = PIN_MIXING_PUMP_PRIME_CONTROL,
  .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
  .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
};

PumpMasterflex mixing_pump = PumpMasterflex(mixing_pump_interface);
// RotaryValve rotary_valve = RotaryValve(PIN_ROTARY_VALVE_RX,PIN_ROTARY_VALVE_TX,16,9600);
Mixer mixer = Mixer(PIN_MIXER_ENABLE);
TempSensorMAX31855 temp_sensor;
Lighter lighter = Lighter(PIN_LIGHTER_INIT,PIN_LIGHTER_ENABLE);

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  // setup Serial communication to computer. Baud rate 115200 bps
  Serial.begin(115200);
  // Connect and initialize Masterflex pump
  Serial.println("SYSTEM INITIALIZING...");
  mixing_pump.Connect();
  mixing_pump.Stop();
  mixing_pump.SetDirection(DIR_CW);
  mixing_pump.SetTubeSize(16);
  mixing_pump.SetMaxVoltageLevel(5000);
  mixing_pump.SetMinVoltageLevel(100);
  mixing_pump.SetMinSpeed(0);
  mixing_pump.SetMaxSpeed(37700);
  mixing_pump.SetSpeed(25000);
  mixing_pump.PipeSetVol(2000);
  // Connect temperature sensor
  temp_sensor.Connect();
  // Connect lighter
  lighter.Connect();
  // Connecct rotary valve
  // rotary_valve.Connect();
  // Connect mixer
  mixer.Connect();
  // sensor.begin();
  delay(1000);
  Serial.println(temp_sensor.readCelsius());
  Serial.println("SYSTEM READY");
}

cmd_t command;
resp_t response;
/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  // reset response and command each iteration
  response.source = DEVICE_NONE;
  command.target = DEVICE_NONE;
  command.instruction = "";
  command.parameter = 0;
  // new command is received
  if (GetCommand(&command) == CMD_RECEIVED)
  {
    Serial.println("Active command");
    // filter command target
    if (command.target.equals(DEVICE_MIXPUMP))
    {
      response  = MasterflexPumpController(&mixing_pump, command);
    }
    else if (command.target.equals(DEVICE_MIXER))
    {
      response = MixerController(&mixer, command);
    }
    else if (command.target.equals(DEVICE_TEMPSENSOR))
    {
      response = TempSensorController(&temp_sensor, command);
    }
    else if (command.target.equals(DEVICE_LIGHTER))
    {
      response = LighterController(&lighter, command);
    }
    else if (command.target.equals(DEVICE_ROTARYVALVE))
    {
      // response = RotaryValveController(&rotary_valve, command);
    }
    else {
      // no command target found -> send back system error with invalid target error code
      response.source = DEVICE_SYSTEM;
      response.error_code = RESP_GENERAL_ERROR_INVALID_TARGET;
      response.data = 0;
      response.type = RESP_TYPE_INVALID;
    }
  }
  else {
    // check for periodic tasks (background)
    // Serial.println("Run peridic tasks");
    response = TempSensorController(&temp_sensor, command);
  }
  // only send back response if source is different than none
  if (response.source != DEVICE_NONE && response.type != RESP_TYPE_NOTHING)
  {
    // Serial.println("sending response...");
    SendResponse(response);
  }
  delay(100);
}