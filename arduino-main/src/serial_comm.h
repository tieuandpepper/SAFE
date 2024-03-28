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

#define BUFFER_SIZE         64
#define CMD_RECEIVED 1
#define CMD_NOTHING 0
#define CMD_INVALID -99

// define reponse ID
#define RESP_INVALID        "INVALID"
#define RESP_VALID           "VALID"


// Command structure: <TARGET>,<COMMAND>,<OPERAND>.
typedef struct cmd_type {
  String target;
  String command_id;
  long operand = 0;
} cmd_t;

// Response structure: RESP,<RESP_ID>,<SOURCE>,<DATA>.
typedef struct resp_type {
  String resp_id;
  String source;
  int32_t data = 0;
} resp_t;

uint8_t GetCommand(cmd_t* command);
uint8_t SendResponse(resp_t response);

#endif