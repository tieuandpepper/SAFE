#include "device_controller.h"

/**
 * @brief 
 * 
 * @param pump 
 * @param command 
 * @return int 
 */
int MixPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond)
{  
  int ret_val = RESP_ACK_VOID;
  respond->source = MIXPUMP;
  if (command->id.equals(MIXPUMP_START))
  {
    pump->Start();
  }
  else if (command->id.equals(MIXPUMP_STOP))
  {
    pump->Stop();
  }
  else if (command->id.equals(MIXPUMP_CW))
  {
    pump->SetDirection(DIR_CW);
  }
  else if (command->id.equals(MIXPUMP_CCW))
  {
    pump->SetDirection(DIR_CCW);
  }
  else if (command->id.equals(MIXPUMP_GETSPEED))
  {
    pump->GetSpeed();
  }
  else if (command->id.equals(MIXPUMP_SETSPEED))
  {
    pump->SetSpeed(command->operand);
  }
  else if (command->id.equals(MIXPUMP_DISPENSE))
  {
    pump->Dispense(command->operand);
  }
  else if (command->id.equals(MIXPUMP_GETSPEEDSETTING))
  {
    pump->GetSpeedSetting();
  }
  else if (command->id.equals(MIXPUMP_SETMAXSPEED))
  {
    pump->SetMaxSpeed(command->operand);
  }
  else if (command->id.equals(MIXPUMP_SETMINSPEED))
  {
    pump->SetMinSpeed(command->operand);
  }
  else if (command->id.equals(MIXPUMP_GETMAXSPEED))
  {
    pump->GetMaxSpeed();
  }
  else if (command->id.equals(MIXPUMP_GETMINSPEED))
  {
    pump->GetMinSpeed();
  }
  else if (command->id.equals(MIXPUMP_PIPESETVOL))
  {
    pump->PipeSetVol(command->operand);
  }
  else {
    ret_val = CMD_INVALID;
  }
  // Serial.println("No matching command");
  return ret_val;
}

/**
 * @brief 
 * 
 * @param mixer 
 * @param command 
 * @return int 
 */
int MixerController(Mixer * mixer, cmd_t * command, resp_t* respond)
{
  uint8_t ret_val = CMD_RECEIVED;
  if (command->id.equals(MIXER_START))
  {
    mixer->Start();
  }
  else if (command->id.equals(MIXER_STOP))
  {
    mixer->Stop();
  }
  else if (command->id.equals(MIXER_RUN))
  {
    mixer->Run(command->operand);
  }
  else {
    ret_val = CMD_INVALID;
  }
  return ret_val;
}

/**
 * @brief 
 * 
 * @param sensor 
 * @param command 
 * @return int 
 */
int TempSensorController(TempSensorMAX31855 * sensor, cmd_t * command, resp_t* respond)
{
  uint8_t ret_val = CMD_RECEIVED;
  if (command->id.equals(TEMPSENSOR_READ_ONCE))
  {
    sensor->ReadSensor()
  }
  else {
    ret_val = CMD_INVALID;
  }
  return ret_val;
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