#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include "Arduino.h"
#include "serial_comm.h"
#include "device/mixer.h"
#include "device/pump_masterflex.h"
#include "device/rotary_valve.h"
#include "device/temp_sensor.h"
#include "device/ezo_pump.h"

// define target
#define MIXPUMP             "MIXPUMP"
#define MIXER               "MIXER"
#define ROTARYVALVE         "ROTVALVE"
#define TEMPSENSOR          "TEMPSENS"
#define LIGHTER             "LIGHTER"
#define TRANSFERPUMP        "XFERPUMP"

// define command ID
#define MIXPUMP_START          "START"
#define MIXPUMP_STOP           "STOP"
#define MIXPUMP_CW             "CLOCK"
#define MIXPUMP_CCW            "COUNTERCLOCK"
#define MIXPUMP_DISPENSE       "DISPENSE"
#define MIXPUMP_GETSPEED       "GET"
#define MIXPUMP_SETSPEED       "SET"
#define MIXPUMP_GETSPEEDSETTING "GETSETTING"
#define MIXPUMP_SETMAXSPEED    "SETMAX"
#define MIXPUMP_SETMINSPEED    "SETMIN"
#define MIXPUMP_GETMAXSPEED    "GETMAX"
#define MIXPUMP_GETMINSPEED    "GETMIN"
#define MIXPUMP_PIPESETVOL     "PIPESET"

#define MIXER_START         "START"
#define MIXER_STOP          "STOP"
#define MIXER_RUN           "RUN"

#define TEMPSENSOR_READ_DURATION_MS   "READMS"
#define TEMPSENSOR_READ_ONCE          "READ"

#define ROTARYVALVE_GET_VERSION         "GETVER"
#define ROTARYVALVE_CHANGE_PORT         "PORT"
#define ROTARYVALVE_GET_VERSION         "GETVER"

uint8_t MixPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond);
uint8_t MixerController(Mixer * mixer, cmd_t * command, resp_t* respond);
uint8_t TempSensorController(TempSensorMAX31855 * sensor, cmd_t * command, resp_t* respond);
uint8_t RotaryValveController(RotaryValve * valve, cmd_t * command, resp_t* respond);
uint8_t TransferPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond);

#endif