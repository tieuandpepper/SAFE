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
  bool read_cont_flag_on = sensor->ContinuousFlagOn();
  response->source = TEMPSENSOR;
  response->data = 0;
  if (command->instruction.equals(TEMPSENSOR_READ_ONCE))
  {
    response->error_code = sensor->ReadSensor(&data);
    response->data = data;
    response->type = RESP_TYPE_FEEDBACK;
  }
  else if (command->instruction.equals(TEMPSENSOR_READ_DURATION_MS) && !read_cont_flag_on)
  {
    response->error_code = sensor->InitializeReadContinuous(command->parameter);
    response->type = RESP_TYPE_VALID;
  }
  else if (read_cont_flag_on)
  {
    response->error_code = sensor->ReadSensorContinous(&data);
    response->data = data;
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
 * @param valve 
 * @param command 
 * @return int 
 */
int RotaryValveController(RotaryValve * valve, cmd_t * command, resp_t* respond)
{
  return 0;
}

/**
 * @brief 
 * 
 * @param pump 
 * @param command 
 * @return int 
 */
int TransferPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond)
{
  return 0;
}