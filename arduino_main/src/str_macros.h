/**
 * @file str_macros.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STR_MACROS_H
#define STR_MACROS_H


// Component
#define CPNT_MIX_PUMP           "MIXPUMP"
#define CPNT_MIXER              "MIXER"
#define CPNT_ROTARY_VALVE       "ROTVALVE"
#define CPNT_LIGHTER            "LIGHTER"
#define CPNT_TEMP_SENSOR        "TEMP"
#define CPNT_TRANSFER_PUMP      "TRANSPUMP"


// Mixing Pump command
#define CMD_MIX_PUMP_STOP           "STOP"
#define CMD_MIX_PUMP_START          "START"
#define CMD_MIX_PUMP_DIRECTION      "DIRECTION"
#define CMD_MIX_PUMP_GET_SPEED      "GETSPEED"
#define CMD_MIX_PUMP_SET_SPEED      "SETSPEED"
#define CMD_MIX_PUMP_GET_SETTING    "GETSETTING"
#define CMD_MIX_PUMP_SET_MAX        "SETMAX"
#define CMD_MIX_PUMP_SET_MIN        "SETMIN"
#define CMD_MIX_PUMP_GET_MAX        "GETMAX"
#define CMD_MIX_PUMP_GET_MIN        "GETMIN"
#define CMD_MIX_PUMP_SET_PIPE_VOL   "SETPIPE"
#define CMD_MIX_PUMP_DISPENSE_UL    "DISPENSE"

// Mixer command
#define CMD_MIXER_STOP              "STOP"
#define CMD_MIXER_START             "START"
#define CMD_MIXER_RUN_PERIOD_MS     "RUN_MS"

// Temperature sensor
#define CMD_TEMP_SENSOR_READ        "READ"
#define CMD_TEMP_SENSOR_BEGIN       "BEGIN"

// Lighter
#define CMD_LIGHTER_STOP            "STOP"
#define CMD_LIGHTER_RUN             "RUN"



#endif