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

// define reponse type
#define RESP_TYPE_NACK "NACK"
#define RESP_TYPE_ACK  "ACK"
#define RESP_TYPE_FEEDBACK "FEEDBACK"

// define response data (for ACK)
#define RESP_ACK_VOID     0
// #define RESP_ACK_RECEIVED 1
// #define RESP_ACK_

// define response data (for NACK)
#define RESP_NACK_VOID    0

// define response data for FEEDBACK
#define RESP_FB_VOID      0


// Command structure: <TARGET>,<INSTRUCTION>,<OPERAND>.
typedef struct command_type {
  String target;
  String command_id;
  int operand = 0;
} cmd_t;

// Response structure: RESP,<TYPE>,<SOURCE>,<DATA>.
typedef struct resp_type {
  String type;
  String source;
  int data = 0;
} resp_t;

int GetCommand(cmd_t* command);
int SendResponse(resp_t response);

#endif