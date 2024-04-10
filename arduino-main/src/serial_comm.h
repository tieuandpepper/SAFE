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
#define CMD_NOTHING 3
#define CMD_INVALID 7

// define reponse ID
#define RESP_FB_INVALID        "INVALID"
#define RESP_FB_VALID           "VALID"
#define RESP_FB_VOID            "VOID"


// Command structure: <TARGET>,<INSTRUCTION>,<OPERAND>.
typedef struct command_type {
  String target;
  String instruction;
  long operand = 0;
} command_t;

// Response structure: RESP,<FEEDBACK>,<SOURCE>,<DATA>.
typedef struct respond_type {
  String feedback;
  String source;
  String data;
} respond_t;

uint8_t GetCommand(command_t* command);
uint8_t SendResponse(respond_t response);

#endif