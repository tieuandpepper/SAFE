#include "task_comm.h"

/// @brief 
/// @param mixer 
/// @param command 
/// @return 
int32_t MixerController(mixer_t * mixer, cmd_t * command)
{
  if (command->cmd_id.equals(CMD_MIXER_START))
  {
    return MixerStart(mixer);
  }
  if (command->cmd_id.equals(CMD_MIXER_STOP))
  {
    return MixerStop(mixer);
  }
  if (command->cmd_id.equals(CMD_MIXER_RUN_PERIOD_MS))
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
  // extract component ID
  uint8_t first_idx = 0;
  uint8_t last_idx = buffer.indexOf(',',first_idx);
  if (first_idx >= last_idx){
    return CMD_NOTHING;
  }
  command->cpnt = buffer.substring(first_idx,last_idx);
  // Serial.print("cpnt="); Serial.print(command->cpnt);
  // extract command ID
  first_idx = last_idx + 1;
  last_idx = buffer.indexOf(',',first_idx);
  // cannot find the comma (no operand)
  if (last_idx == -1)
  {
    last_idx = buffer.length();
  }

  if (first_idx >= last_idx){
    command->cpnt = "";
    return CMD_NOTHING;
  }
  command->cmd_id = buffer.substring(first_idx,last_idx);
  // Serial.print(" | Command="); Serial.print(command->cmd_id);
  // extract operand ID
  first_idx = last_idx + 1;
  last_idx = buffer.length();
  if (first_idx < last_idx)
  {
    command->operand = buffer.substring(first_idx, last_idx).toInt();
  }
  // Serial.print(" | Operand="); Serial.println(command->operand);
  return CMD_RECEIVED;
}

/// @brief 
/// @param response 
/// @return 
void SendResponse(resp_t response)
{
  String buffer = "RESP,";
  buffer += response.resp_id;
  buffer += ",";
  buffer += response.cpnt;
  buffer += ",";
  buffer += response.cmd_id;
  buffer += ",";
  buffer += response.data;
  buffer += ".";
  Serial.println(buffer);
}
