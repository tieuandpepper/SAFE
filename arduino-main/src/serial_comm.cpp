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
int GetCommand(cmd_t* command)
{
  String buffer =  "";
  if (!Serial.available())
  {
    return CMD_UNAVAILABLE;
  }
  Serial.println("serial data received");
  command->size = Serial.readBytesUntil('>',command->buffer,64);
  command->buffer[command->size++] = '>';
  buffer = String(command->buffer);
  Serial.println(command->buffer);
  if (buffer.indexOf('<') != 0)
  {
    return CMD_INVALID;
  }
  buffer = buffer.substring(buffer.indexOf('<')+1);
  Serial.println(buffer);
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

/// @brief 
/// @param response 
/// @return 
int SendResponse(resp_t * response)
{
  if (response->source == DEVICE_NONE || response->type == RESP_TYPE_NOTHING)
  {
    return RESP_UNAVAILABLE;
  }
  String buffer = "<RESP;";
  String error = "";
  buffer += response->source;
  buffer += ";";
  buffer += response->type;
  buffer += ";";
  error = String(response->error_code,HEX);
  error.toUpperCase();
  buffer += (error);
  if (response->data.length() > 0)
  {
    buffer += ";";
    buffer += response->data;
  }
  buffer += ">";
  Serial.println(buffer);
  response->size = buffer.length();
  buffer.toCharArray(response->buffer,response->size);
  return RESP_SENT;
}

void PrintCommand(cmd_t command)
{
  Serial.print("<CMD;"); Serial.print(command.target);
  Serial.print(";"); Serial.print(command.instruction);
  Serial.print(";"); Serial.print(command.parameter);
  Serial.println(">");
}
