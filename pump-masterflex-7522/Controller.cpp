#include "Controller.h"

/// @brief 
/// @param command 
/// @return 
uint32_t Controller(PumpMasterflex pumps[PUMP_COUNT], cmd_t* command)
{
  uint8_t pump_idx = 0;
  if (command->target.equals(PUMP_1))
  {
    pump_idx = 1;
  }
  else if (command->target.equals(PUMP_2))
  {
    pump_idx = 2;
  }
  else {
    return CMD_INVALID;
  }
  
  if (command->command.equals(PUMP_START))
  {
    return pumps[pump_idx].Start();
  }
  else if (command->command.equals(PUMP_STOP))
  {
    return pumps[pump_idx].Stop();
  }
  else if (command->command.equals(PUMP_CW))
  {
    return pumps[pump_idx].SetDirection(DIR_CW);
  }
  else if (command->command.equals(PUMP_CCW))
  {
    return pumps[pump_idx].SetDirection(DIR_CCW);
  }
  else if (command->command.equals(PUMP_GETSPEED))
  {
    return pumps[pump_idx].GetSpeed();
  }
  else if (command->command.equals(PUMP_SETSPEED))
  {
    return pumps[pump_idx].SetSpeed(command->operand);
  }
  else if (command->command.equals(PUMP_DISPENSE))
  {
    return pumps[pump_idx].SetSpeed(command->operand);
  }
  else {
    Serial.println("No matching command");
    return CMD_INVALID;
  }
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
  buffer.toUpperCase();
  Serial.println(buffer);
  // extract target ID
  uint8_t first_idx = 0;
  uint8_t last_idx = buffer.indexOf(',',first_idx);
  if (first_idx >= last_idx){
    return CMD_NOTHING;
  }
  command->target = buffer.substring(first_idx,last_idx);
  Serial.print("Target: "); Serial.println(command->target);
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
  command->command = buffer.substring(first_idx,last_idx);
  Serial.print("Command: "); Serial.println(command->command);
  // extract operand ID
  first_idx = last_idx + 1;
  last_idx = buffer.length();
  if (first_idx < last_idx)
  {
    command->operand = buffer.substring(first_idx, last_idx).toInt();
  }
  Serial.print("Operand: "); Serial.println(command->operand);
  return CMD_RECEIVED;
}
