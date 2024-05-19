/**
 * @file serial_comm.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SERIAL_COMM_H
#define SERAIL_COMM_H

#include "Arduino.h"
#include "command_list.h"

#define BUFFER_SIZE         64
#define CMD_RECEIVED        0xCD01
#define CMD_UNAVAILABLE     0xCDE2
#define CMD_INVALID         0xCDE3

// define reponse type
#define RESP_TYPE_VALID    "VALID"
#define RESP_TYPE_INVALID  "INVALID"
#define RESP_TYPE_FEEDBACK "FEEDBACK"
#define RESP_TYPE_UNSUPPORTED "UNSUPPORTED"
#define RESP_TYPE_NOTHING  "NOTHING" // use to signal there is no meaningful response to send

// Command structure: <TARGET>,<INSTRUCTION>,<OPERAND>.
typedef struct command_type {
  String target;
  String instruction;
  uint32_t parameter = 0;
} cmd_t;

// Response structure: RESP,<SOURCE>,<TYPE>,<ERROR_CODE>,<DATA>.
typedef struct resp_type {
  String source;
  String type;
  uint16_t error_code;
  String data = "";
} resp_t;

int GetCommand(cmd_t* command);
int SendResponse(resp_t response);
void PrintCommand(cmd_t command);

#endif