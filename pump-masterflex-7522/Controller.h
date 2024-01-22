#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "PumpMasterflex.h"
#include "mixer.h"

#define BUFFER_SIZE         64
#define CMD_RECEIVED 1
#define CMD_NOTHING 0
#define CMD_INVALID -99
#define PUMP_COUNT  2
// define target
#define PUMP_1              "PUMP1"
#define PUMP_2              "PUMP2"
#define MIXER               "MIXER"

// define command ID
#define PUMP_START          "START"
#define PUMP_STOP           "STOP"
#define PUMP_CW             "CLOCK"
#define PUMP_CCW            "COUNTERCLOCK"
#define PUMP_DISPENSE       "DISPENSE"
#define PUMP_GETSPEED       "GET"
#define PUMP_SETSPEED       "SET"
#define PUMP_GETSPEEDSETTING "GETSETTING"
#define PUMP_SETMAXSPEED    "SETMAX"
#define PUMP_SETMINSPEED    "SETMIN"
#define PUMP_GETMAXSPEED    "GETMAX"
#define PUMP_GETMINSPEED    "GETMIN"
#define PUMP_PIPESETVOL     "PIPESET"
#define MIXER_START         "START"
#define MIXER_STOP          "STOP"
#define MIXER_RUN           "RUN"

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

int32_t PumpController(PumpMasterflex pumps[PUMP_COUNT], cmd_t* command);
int32_t MixerController(mixer_t * mixer, cmd_t * command);
uint8_t GetCommand(cmd_t* command);
uint8_t SendResponse(resp_t response);


#endif