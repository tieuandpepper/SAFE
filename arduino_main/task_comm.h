#ifndef TASK_COMM_H
#define TASK_COMM_H

#include "Arduino.h"
#include "src/pump_masterflex.h"
#include "src/mixer_generic.h"
#include "str_macros.h"

#define BUFFER_SIZE         64
#define CMD_RECEIVED 1
#define CMD_NOTHING 0
#define CMD_INVALID -99
#define PUMP_COUNT  2

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

int32_t PumpController(PumpMasterflex* pump, cmd_t* command);
int32_t MixerController(mixer_t * mixer, cmd_t * command);
uint8_t GetCommand(cmd_t* command);
uint8_t SendResponse(resp_t response);


#endif