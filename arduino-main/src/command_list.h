/**
 * @file command_list.h
 * @author your name (you@domain.com)
 * @brief containing the list of supported commands (targets and instructions)
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H


// define target/source for command/response
#define DEVICE_MASTERFLEXPUMP      "MIXPUMP"
#define DEVICE_MIXER               "MIXER"
#define DEVICE_ROTARYVALVE         "ROTARYVALVE"
#define DEVICE_TEMPSENSOR          "TEMPSENSOR"
#define DEVICE_LIGHTER             "LIGHTER"
#define DEVICE_EZOPUMP             "TRANSFERPUMP"
#define DEVICE_NONE                "NONE"
#define DEVICE_SYSTEM              "SYSTEM"

// define command instruction
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

#define TEMPSENSOR_READ_DURATION_MS   "READTIME"
#define TEMPSENSOR_READ_PERIOD_MS     "PERIODIC"
#define TEMPSENSOR_READ_ONCE          "READ"
#define TEMPSENSOR_READ_STOP          "STOP"

#define LIGHTER_IGNITE                  "IGNITE"

#define ROTARYVALVE_SET_ADDR            "SETADDRESS"
#define ROTARYVALVE_SET_BAUD_RATE       "SETBAUDRATE"
#define ROTARYVALVE_FACTORY_RESET       "FACTORYRESET"
#define ROTARYVALVE_GET_ADDR            "GETADDRESS"
#define ROTARYVALVE_GET_BAUD_RATE       "GETBAUDRATE"
#define ROTARYVALVE_GET_MOTOR_STATUS    "MOTORSTATUS"
#define ROTARYVALVE_GET_VERSION         "VERSION"
#define ROTARYVALVE_RESET_ORIGIN        "MOVEORIGIN"
#define ROTARYVALVE_MOVE_AUTO           "MOVEAUTO"
#define ROTARYVALVE_STOP                "STOP"

#define EZOPUMP_DISPENSE_CONTINUOUS     "START"
#define EZOPUMP_DISPENSE_STOP           "STOP"
#define EZOPUMP_DISPENSE_PAUSE          "PAUSE"
#define EZOPUMP_DISPENSE_AMOUNT         "DISPENSE"
#define EZOPUMP_DISPENSE_READING        "READING"
#define EZOPUMP_DISPENSE_GET_VOL        "AMOUNT"
#define EZOPUMP_DISPENSE_STATUS         "STATUS"
#define EZOPUMP_CALIBRATE               "CALIBRATE"
#define EZOPUMP_VOLTAGE                 "VOLTAGE"
#define EZOPUMP_DEVICE_INFO             "INFO"
#define EZOPUMP_DIRECTION_CHANGE        "DIRECTION"
#define EZOPUMP_CALIBRATE_RESET         "CALRESET"


#endif