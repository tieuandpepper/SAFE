#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include "Arduino.h"
#include "serial_comm.h"
#include "device/mixer.h"
#include "device/pump_masterflex.h"
#include "device/rotary_valve.h"

// define target
#define MIXINGPUMP          "MIXPUMP"
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


int32_t PumpController(PumpMasterflex* pump, cmd_t* command);
int32_t MixerController(mixer_t * mixer, cmd_t * command);


#endif