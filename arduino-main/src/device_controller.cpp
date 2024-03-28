#include "device_controller.h"

/// @brief 
/// @param command 
/// @return 
int32_t PumpController(PumpMasterflex* pump, cmd_t* command)
{
  uint8_t pump_idx = 0;
  
  if (command->command_id.equals(PUMP_START))
  {
    return pump->Start();
  }
  if (command->command_id.equals(PUMP_STOP))
  {
    return pump->Stop();
  }
  if (command->command_id.equals(PUMP_CW))
  {
    return pump->SetDirection(DIR_CW);
  }
  if (command->command_id.equals(PUMP_CCW))
  {
    return pump->SetDirection(DIR_CCW);
  }
  if (command->command_id.equals(PUMP_GETSPEED))
  {
    return pump->GetSpeed();
  }
  if (command->command_id.equals(PUMP_SETSPEED))
  {
    return pump->SetSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_DISPENSE))
  {
    return pump->Dispense(command->operand);
  }
  if (command->command_id.equals(PUMP_GETSPEEDSETTING))
  {
    return pump->GetSpeedSetting();
  }
  if (command->command_id.equals(PUMP_SETMAXSPEED))
  {
    return pump->SetMaxSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_SETMINSPEED))
  {
    return pump->SetMinSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_GETMAXSPEED))
  {
    return pump->GetMaxSpeed();
  }
  if (command->command_id.equals(PUMP_GETMINSPEED))
  {
    return pump->GetMinSpeed();
  }
    // Serial.println("No matching command");
    return CMD_INVALID;
}

/// @brief 
/// @param mixer 
/// @param command 
/// @return 
int32_t MixerController(mixer_t * mixer, cmd_t * command)
{
  if (command->command_id.equals(MIXER_START))
  {
    return MixerStart(mixer);
  }
  if (command->command_id.equals(MIXER_STOP))
  {
    return MixerStop(mixer);
  }
  if (command->command_id.equals(MIXER_RUN))
  {
    return MixerRun(mixer, command->operand);
  }
  return CMD_INVALID;
}