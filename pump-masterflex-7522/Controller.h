#ifndef Controller_h
#define Controller_h

#include "PumpMasterflex.h"
#include "Arduino.h"

#define BUFFER_SIZE         64
#define CMD_RECEIVED 1
#define CMD_NOTHING 0
#define CMD_INVALID 99
#define PUMP_COUNT  2
// define target
#define PUMP_1              "PUMP1"
#define PUMP_2              "PUMP2"

// define command
#define PUMP_START          "START"
#define PUMP_STOP           "STOP"
#define PUMP_CW             "CW"
#define PUMP_CCW            "CCW"
#define PUMP_DISPENSE       "DISP"
#define PUMP_GETSPEED       "GET"
#define PUMP_SETSPEED       "SET"

// Command structure: <TARGET>,<COMMAND>,<OPERAND>.
typedef struct cmd_type {
  String target;
  String command;
  long operand = 0;
} cmd_t;

uint32_t Controller(PumpMasterflex pumps[PUMP_COUNT], cmd_t* command);
uint8_t GetCommand(cmd_t* command);
// uint8_t ReadData(char);






#endif