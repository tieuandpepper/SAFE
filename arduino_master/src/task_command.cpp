#include "task_command.h"

/// @brief 
/// @param command 
/// @return 
int32_t PumpController(PumpMasterflex pumps[PUMP_COUNT], cmd_t* command)
{
  uint8_t pump_idx = 0;
  if (command->target.equals(PUMP_1))
  {
    pump_idx = 0;
  }
  else if (command->target.equals(PUMP_2))
  {
    pump_idx = 1;
  }
  else {
    return CMD_INVALID;
  }
  
  if (command->command_id.equals(PUMP_START))
  {
    return pumps[pump_idx].Start();
  }
  if (command->command_id.equals(PUMP_STOP))
  {
    return pumps[pump_idx].Stop();
  }
  if (command->command_id.equals(PUMP_CW))
  {
    return pumps[pump_idx].SetDirection(DIR_CW);
  }
  if (command->command_id.equals(PUMP_CCW))
  {
    return pumps[pump_idx].SetDirection(DIR_CCW);
  }
  if (command->command_id.equals(PUMP_GETSPEED))
  {
    return pumps[pump_idx].GetSpeed();
  }
  if (command->command_id.equals(PUMP_SETSPEED))
  {
    return pumps[pump_idx].SetSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_DISPENSE))
  {
    return pumps[pump_idx].Dispense(command->operand);
  }
  if (command->command_id.equals(PUMP_GETSPEEDSETTING))
  {
    return pumps[pump_idx].GetSpeedSetting();
  }
  if (command->command_id.equals(PUMP_SETMAXSPEED))
  {
    return pumps[pump_idx].SetMaxSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_SETMINSPEED))
  {
    return pumps[pump_idx].SetMinSpeed(command->operand);
  }
  if (command->command_id.equals(PUMP_GETMAXSPEED))
  {
    return pumps[pump_idx].GetMaxSpeed();
  }
  if (command->command_id.equals(PUMP_GETMINSPEED))
  {
    return pumps[pump_idx].GetMinSpeed();
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

/// @brief get data from buffer and parse it into command struct
/// @param command 
/// @return 0 if no command and 1 for valid command
uint8_t GetCommand(cmd_t* command)
{
  if (!Serial.available())
  {
    return CMD_NOTHING;
  }
  String buffer = Serial.readStringUntil('.');
  // trim whitespace and turn all characters uppercase
  buffer.trim();
  buffer.replace(" ", "");
  buffer.toUpperCase();
  Serial.println(buffer);
  // extract target ID
  uint8_t first_idx = 0;
  uint8_t last_idx = buffer.indexOf(',',first_idx);
  if (first_idx >= last_idx){
    return CMD_NOTHING;
  }
  command->target = buffer.substring(first_idx,last_idx);
  Serial.print("Target="); Serial.print(command->target);
  // extract command ID
  first_idx = last_idx + 1;
  last_idx = buffer.indexOf(',',first_idx);
  // cannot find the comma (no operand)
  if (last_idx == -1)
  {
    last_idx = buffer.length();
  }

  if (first_idx >= last_idx){
    command->target = "";
    return CMD_NOTHING;
  }
  command->command_id = buffer.substring(first_idx,last_idx);
  Serial.print(" | Command="); Serial.print(command->command_id);
  // extract operand ID
  first_idx = last_idx + 1;
  last_idx = buffer.length();
  if (first_idx < last_idx)
  {
    command->operand = buffer.substring(first_idx, last_idx).toInt();
  }
  Serial.print(" | Operand="); Serial.println(command->operand);
  return CMD_RECEIVED;
}

/// @brief 
/// @param response 
/// @return 
uint8_t SendResponse(resp_t response)
{
  String buffer = "RESP,";
  buffer += response.resp_id;
  buffer += ",";
  buffer += response.source;
  buffer += ",";
  buffer += response.data;
  buffer += ".";
  Serial.println(buffer);
}

