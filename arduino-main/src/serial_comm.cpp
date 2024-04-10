/**
 * @file serial_comm.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "serial_comm.h"

/// @brief get data from buffer and parse it into command struct
/// @param command 
/// @return 0 if no command and 1 for valid command
uint8_t GetCommand(command_t* command)
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
  command->instruction = buffer.substring(first_idx,last_idx);
  Serial.print(" | Command="); Serial.print(command->instruction);
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
uint8_t SendResponse(respond_t response)
{
  String buffer = "RESP,";
  buffer += response.feedback;
  buffer += ",";
  buffer += response.source;
  buffer += ",";
  buffer += response.data;
  buffer += ".";
  Serial.println(buffer);
}

