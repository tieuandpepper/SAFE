#ifndef TASK_COMM_H
#define TASK_COMM_H

#include "Arduino.h"
#include "driver/mixer_generic.h"
#include "str_macros.h"

#define CMD_RECEIVED 1
#define CMD_NOTHING 0
#define CMD_INVALID -99

// define reponse ID
#define RESP_ID_INVALID           "INVALID"
#define RESP_ID_SUCCESS           "SUCCESS"
#define RESP_ID_FAILED            "FAILED"
#define RESP_ID_DATA              "DATA"
#define RESP_ID_RECEIVED          "RECEIVED"
#define RESP_ID_PUMP_DISP_START   "START"
#define RESP_ID_PUMP_DISP_STOP    "STOP"


// Command structure: <COMPONENT>,<COMMAND>,<OPERAND>.
typedef struct cmd_type {
  String cpnt;
  String cmd_id;
  long operand = 0;
} cmd_t;

// Response structure: RESP,<RESP_ID>,<SOURCE>,<DATA>.
typedef struct resp_type {
  String resp_id;
  String cpnt;
  String cmd_id;
  int32_t data = 0;
} resp_t;

int32_t MixerController(mixer_t * mixer, cmd_t * command);
uint8_t GetCommand(cmd_t* command);
void SendResponse(resp_t response);

#endif