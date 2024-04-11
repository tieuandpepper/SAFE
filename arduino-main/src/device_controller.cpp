#include "device_controller.h"

/**
 * @brief 
 * 
 * @param pump 
 * @param command 
 * @param response 
 */
void MixPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* response)
{
  response->source = MIXPUMP;
  response->type = RESP_TYPE_VALID;
  response->data = 0;
  response->error_code = RESP_FEEDBACK_VOID;
  if (command->instruction.equals(MIXPUMP_START))
  {
    response->error_code = pump->Start();
  }
  else if (command->instruction.equals(MIXPUMP_STOP))
  {
    response->error_code = pump->Stop();
  }
  else if (command->instruction.equals(MIXPUMP_CW))
  {
    response->error_code = pump->SetDirection(DIR_CW);
  }
  else if (command->instruction.equals(MIXPUMP_CCW))
  {
    response->error_code = pump->SetDirection(DIR_CCW);
  }
  else if (command->instruction.equals(MIXPUMP_SETSPEED))
  {
    response->error_code = pump->SetSpeed(command->operand);
  }
  else if (command->instruction.equals(MIXPUMP_DISPENSE))
  {
    response->error_code = pump->Dispense(command->operand);
  }
  else if (command->instruction.equals(MIXPUMP_SETMAXSPEED))
  {
    response->error_code = pump->SetMaxSpeed(command->operand);
  }
  else if (command->instruction.equals(MIXPUMP_SETMINSPEED))
  {
    response->error_code = pump->SetMinSpeed(command->operand);
  }
  else if (command->instruction.equals(MIXPUMP_PIPESETVOL))
  {
    response->error_code = pump->PipeSetVol(command->operand);
  }
  else if (command->instruction.equals(MIXPUMP_GETSPEED))
  {
    response->data = pump->GetSpeed();
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(MIXPUMP_GETMAXSPEED))
  {
    response->data = pump->GetMaxSpeed();
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(MIXPUMP_GETMINSPEED))
  {
    response->data = pump->GetMinSpeed();
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(MIXPUMP_GETSPEEDSETTING))
  {
    response->data = pump->GetSpeedSetting();
    response->type = RESP_TYPE_FEEDBACK;
  }
  else {
    response->type = RESP_TYPE_INVALID;
    response->error_code = RESP_FEEDBACK_ERR_INVALID;
  }
}

/**
 * @brief 
 * 
 * @param mixer 
 * @param command 
 * @param response 
 */
void MixerController(Mixer * mixer, cmd_t * command, resp_t* response)
{
  response->source = MIXER;
  response->type = RESP_TYPE_VALID;
  response->data = 0;
  if (command->instruction.equals(MIXER_START))
  {
    response->error_code = mixer->Start();
  }
  else if (command->instruction.equals(MIXER_STOP))
  {
    response->error_code = mixer->Stop();
  }
  else if (command->instruction.equals(MIXER_RUN))
  {
    response->error_code = mixer->Run(command->operand);
  }
  else {
    response->type = RESP_TYPE_INVALID;
    response->error_code = RESP_FEEDBACK_ERR_INVALID;
  }
}

/**
 * @brief 
 * 
 * @param sensor 
 * @param command 
 * @param response 
 */
void TempSensorController(TempSensorMAX31855 * sensor, cmd_t * command, resp_t* response)
{
  float data;
  response->source = TEMPSENSOR;
  response->data = 0;
  if (sensor->DurationFlagOn())
  {
    response->error_code = sensor->ReadSensorbyDuration(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (sensor->PeriodFlagOn())
  {
    response->error_code = sensor->ReadSensorbyPeriod(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(TEMPSENSOR_READ_ONCE))
  {
    response->error_code = sensor->ReadSensor(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(TEMPSENSOR_READ_DURATION_MS))
  {
    response->error_code = sensor->ReadSensorbyDuration(command->parameter);
    response->type = RESP_TYPE_VALID;
  }
  else if (command->instruction.equals(TEMPSENSOR_READ_PERIOD_MS))
  {
    response->error_code = sensor->ReadSensorbyPeriod(command->parameter);
    response->type = RESP_TYPE_VALID;
  }
  else if (command->instruction.equals(TEMPSENSOR_READ_STOP))
  {
    response->error_code = sensor->StopRead();
    response->type = RESP_TYPE_VALID;
  }
  else {
    response->type = RESP_TYPE_INVALID;
    response->error_code = RESP_FEEDBACK_ERR_INVALID;
  }
}

/**
 * @brief 
 * 
 * @param valve 
 * @param command 
 * @param response 
 */
void RotaryValveController(RotaryValve * valve, cmd_t * command, resp_t* response)
{
  int data; 
  response->source = ROTARYVALVE;
  response->type = RESP_TYPE_VALID;
  response->data = 0;
  if (command->instruction.equals(ROTARYVALVE_SET_ADDR))
  {
    response->error_code = valve->FactorySetAddr(command->parameter);
  }
  else if (command->instruction.equals(ROTARYVALVE_SET_BAUD_RATE))
  {
    response->error_code = valve->FactorySetBaudRate(command->parameter);
  }
  else if (command->instruction.equals(ROTARYVALVE_FACTORY_RESET))
  {
    response->error_code = valve->FactorySetAutoReset(command->parameter);
  }
  else if (command->instruction.equals(ROTARYVALVE_GET_ADDR))
  {
    response->error_code = valve->QueryAddress(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(ROTARYVALVE_GET_BAUD_RATE))
  {
    response->error_code = valve->QueryBaudRate(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(ROTARYVALVE_GET_MOTOR_STATUS))
  {
    response->error_code = valve->QueryMotorStatus(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(ROTARYVALVE_GET_VERSION))
  {
    response->error_code = valve->QueryCurrVersion(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(ROTARYVALVE_RESET_ORIGIN))
  {
    response->error_code = valve->ActionResetOrigin();
  }
  else if (command->instruction.equals(ROTARYVALVE_MOVE_AUTO))
  {
    response->error_code = valve->ActionMoveAuto(command->parameter);
  }
  else if (command->instruction.equals(ROTARYVALVE_STOP))
  {
    response->error_code = valve->ActionStop();
  }
  else {
    response->type = RESP_TYPE_INVALID;
    response->error_code = RESP_FEEDBACK_ERR_INVALID;
  }
}

/**
 * @brief 
 * 
 * @param pump 
 * @param command 
 * @param response 
 */
void LighterController(Lighter * lighter, cmd_t* command, resp_t* response)
{
  response->source = LIGHTER;
  response->data = 0;
  response->type = RESP_TYPE_VALID;
  if (command->instruction.equals(LIGHTER_IGNITE))
  {
    response->error_code = lighter->Ignite();
  }
  else {
    response->type = RESP_TYPE_INVALID;
    response->error_code = RESP_FEEDBACK_ERR_INVALID;
  }
}