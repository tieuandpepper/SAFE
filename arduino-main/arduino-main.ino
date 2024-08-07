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
#include "src/arduino_pinout.h"
/*----------------------------------------------------------------------------------------------------------*/
// if programming for the central arduino (the one controls most of devices), set this to 1
// if programming for rotary valve/devices that uses UART/serial commns, set this to 0
#define ARDUINO_CENTRAL 1

#if ARDUINO_CENTRAL
MasterflexDB25Interface_t mixing_pump_interface {
  .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
  .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
  .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
  .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
};

PumpMasterflex mixing_pump = PumpMasterflex(mixing_pump_interface);
Mixer mixer = Mixer(PIN_MIXER_ENABLE);
TempSensorMAX31855 temp_sensor;
EZOPump transfer_pump = EZOPump();
ArcLighter lighter = ArcLighter(PIN_LIGHTER_ENABLE,PIN_LIGHTER_CHARGER);
uint8_t i2c_cmd_sent = false;

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  // setup Serial communication to computer. Baud rate 115200 bps
  Serial.begin(115200);
  // Connect and initialize Masterflex pump
  Serial.println("SYSTEM INITIALIZING...");
  // connect mixing pump
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
  // Connect mixer
  mixer.Connect();
  transfer_pump.Connect();
  // Connect lighter
  lighter.Connect();
  lighter.SetChargerPin(PIN_LIGHTER_CHARGER);
  lighter.SetEnablePin(PIN_LIGHTER_ENABLE);
  // start I2C comm for rotary valve arduino
  Wire.begin();
  Serial.println("SYSTEM READY");
}

cmd_t command;
resp_t response;
unsigned long processed_time;
int response_status = RESP_UNAVAILABLE;
/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  // reset response and command each iteration
  response_status = 0;
  response.source = DEVICE_NONE;
  command.target = DEVICE_NONE;
  command.instruction = "";
  command.parameter = 0;
  processed_time = millis();
  // new command is received
  if (GetCommand(&command) == CMD_RECEIVED)
  {
    // Serial.println("Active command");
    // filter command target
    if (command.target.equals(DEVICE_MASTERFLEXPUMP))
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
    else if (command.target.equals(DEVICE_EZOPUMP))
    {
      response = EZOPumpController(&transfer_pump, command);
    }
    else if (command.target.equals(DEVICE_LIGHTER))
    {
      response = ArcLighterController(&lighter, command);
    }
    else if (command.target.equals(DEVICE_ROTARYVALVE))
    {
      // send the command to rotary valve via I2C
      Wire.beginTransmission(I2C_ADDR_ROTARYVALVE_ARDUINO);
      Wire.write(command.buffer,command.size);
      Wire.endTransmission();
      i2c_cmd_sent = true;
      Wire.requestFrom(I2C_ADDR_ROTARYVALVE_ARDUINO,I2C_BUFFER_SIZE);
    }
    else {
      // no command target found -> send back system error with invalid target error code
      response.source = DEVICE_SYSTEM;
      response.error_code = RESP_GENERAL_ERROR_INVALID_TARGET;
      response.data = "";
      response.type = RESP_TYPE_UNSUPPORTED;
    }
    // send back response
    if (response.source != DEVICE_NONE)
    {
      response_status |= SendResponse(&response);
    }
  }
  else {
    // check for periodic tasks (background)
    // Serial.println("Run peridic tasks");
    response = ArcLighterController(&lighter, command);
    response_status |= SendResponse(&response);
    response = TempSensorController(&temp_sensor, command);
    response_status |= SendResponse(&response);
  }
  // process i2c response from rotary valve
  response.size = 0;
  while (Wire.available() && i2c_cmd_sent)
  {
    if (response.size > 0 && response.buffer[response.size] == '>')
    {
      Serial.println(response.buffer);
      response_status |= RESP_SENT;
      break;
    }
    response.buffer[response.size++] = Wire.read();
  }
  // only send back response if source is different than none
  #if ARDUINO_VERBOSE
  if (response_status == RESP_SENT)
  {
    processed_time = millis() - processed_time;
    Serial.print("Processing time [ms]: "); Serial.println(processed_time);
  }
  #endif
  delay(50);
}
#endif


/********************************===============================********************************/
/*==============================|  ROTARY VALVE (UART) ARDUINO  |==============================*/
/********************************===============================********************************/

#if !ARDUINO_CENTRAL
RotaryValve rotary_valve = RotaryValve(PIN_ROTARY_VALVE_RX,PIN_ROTARY_VALVE_TX,16,9600);
cmd_t command;
resp_t response;
String buffer;
bool receivedBuffer = false;
uint8_t counter = 0;
/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  // setup Serial communication to computer. Baud rate 115200 bps
  Serial.begin(115200);
  Serial.println("ROTARY VALVE INITIALIZING...");
  // start as i2c slave
  Wire.begin(I2C_ADDR_ROTARYVALVE_ARDUINO);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  // Connecct rotary valve
  rotary_valve.Connect();
  Serial.println("ROTARY VALVE READY");
}

/// @brief Run after setup(). Will run in loop repeatedly
void loop(){
  if (receivedBuffer && process_command(&command) == CMD_RECEIVED)
  {
    counter = 0;
    do {
      counter++;
      response = RotaryValveController(&rotary_valve, command);
      delay(1000);
    } while (response.error_code != RESP_ROTVALVE_NORMAL_STATUS && response.error_code != RESP_GENERAL_FEEDBACK_SUCCESS && counter < 3);
  }
  delay(20);
}

// receive event handler
void receiveEvent(int num_bytes)
{
  buffer = Wire.readStringUntil('>');
  receivedBuffer = true;
}

// request event handler
void requestEvent()
{
  if (response.source != DEVICE_NONE && response.type != RESP_TYPE_NOTHING)
  {
    SendResponse(&response);
    Wire.write(response.buffer,response.size);
  }
  response.source = DEVICE_NONE;
  response.type = RESP_TYPE_NOTHING;
}

int process_command(cmd_t *command)
{
  if (buffer.indexOf('<') != 0)
  {
    command->target = DEVICE_NONE;
    return;
  }
  buffer = buffer.substring(buffer.indexOf('<')+1);
  // Serial.println(buffer);
  // trim whitespace and turn all characters uppercase
  buffer.trim();
  buffer.replace(" ", "");
  buffer.toUpperCase();
  // extract target ID
  uint16_t first_idx = 0;
  uint16_t last_idx = buffer.indexOf(';',first_idx);
  if (first_idx >= last_idx){
    command->target = DEVICE_NONE;
    return CMD_INVALID;
  }
  command->target = buffer.substring(first_idx,last_idx);
  // Serial.println(command->target);
  // extract command ID
  first_idx = last_idx + 1;
  last_idx = buffer.indexOf(';',first_idx);
  // cannot find the comma (no operand)
  if (last_idx == -1)
  {
    last_idx = buffer.length();
  }

  if (first_idx >= last_idx){
    command->target = DEVICE_NONE;
    return CMD_INVALID;
  }
  command->instruction = buffer.substring(first_idx,last_idx);
  // Serial.println(command->instruction);
  // extract parameter
  first_idx = last_idx + 1;
  last_idx = buffer.length();
  command->parameter = 0;
  if (first_idx < last_idx)
  {
    command->parameter = (int)(buffer.substring(first_idx, last_idx).toInt());
  }
  PrintCommand(*command);
  return CMD_RECEIVED;
}

#endif