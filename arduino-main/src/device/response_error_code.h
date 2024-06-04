/**
 * @file response_error_code.h
 * @author your name (you@domain.com)
 * @brief define response value for each driver. Format shoud be 0xHHHH (2 bytes or 16 bits)
 *        DRIVER FEEDBACK (NORMAL OPERATION): 0xF___
 *        DRIVER ERROR                      : 0xE___
 *        DEVICE ERROR/FEEDBACK             : 0x____ (as specified by manufacturer in manual)
 * @protocol for driver error/feedback
 *        3 half-bytes available (first half-byte is reserved to denote feedback (F) or error (E))
 *        <B0><B1><B2><B3>
 *        B0: error (E) / feedback (F)
 *        B1: origin (Device specific/ general/ system) -> max 14 devices + 1 general + 1 system (each with 16 errors and 16 feedbacks)
 *            C : general-common error, usually from the command, not from the device
 *            A : Arduino problems - system
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RESPONSE_ERROR_CODE_H
#define RESPONSE_ERROR_CODE_H

/* Common/General feedback and error 0xFC0__ or OxEC0__ */ 
#define RESP_GENERAL_FEEDBACK_VOID              0xFC00  //nothing (for data and error_code) to return (some execution has no response)
#define RESP_GENERAL_FEEDBACK_SUCCESS           0xFC01  //successful execution
#define RESP_GENERAL_FEEDBACK_WAITING           0xFC02  // waiting
#define RESP_GENERAL_ERROR_INVALID_TARGET       0xEC01  //invalid command's target
#define RESP_GENERAL_ERROR_INVALID_INSTRUCTION  0xEC02  //invalid command's instruction -> no command execution
#define RESP_GENERAL_ERROR_INVALID_PARAMS       0xEC03  //invalid parameter value

/* TEMPERATURE SENSOR */
#define RESP_TEMPSENSOR_ERR_SENSOR              0xE001

/* Response from ROTARY VALVE software driver */
#define RESP_ROTVALVE_ERR_PARAMS                 0xE101 // Error in user input parameters
#define RESP_ROTVALVE_ERR_CONNECTION             0xE102 // Error in connecting to the valve (no response)
#define RESP_ROTVALVE_ERR_SEND_FAILED            0xE103 // Error in sending command to valve
#define RESP_ROTVALVE_ERR_SEND_OVERFLOW          0xE104 // Too many bytes per command were sent to valve
#define RESP_ROTVALVE_ERR_BUFFER_EMPTY           0xE105 // Not receive any response from valve
#define RESP_ROTVALVE_ERR_BUFFER_MISSING         0xE106 // Missing bytes from valve's response
#define RESP_ROTVALVE_ERR_FRAME                  0xE107 // Wrong message frame (start and stop bytes)
#define RESP_ROTVALVE_ERR_CHECKSUM               0xE108 // Wrong checksum value
#define RESP_ROTVALVE_ERR_INVALID                0xE109 // Invalid value in response from valve

/* Response function code B2 (byte 2) from valve - from datasheet/manual */
#define RESP_ROTVALVE_NORMAL_STATUS              0x0000
#define RESP_ROTVALVE_ERROR_FRAME                0x0001
#define RESP_ROTVALVE_ERROR_PARAMS               0x0002
#define RESP_ROTVALVE_ERROR_OPTOCOUPLER          0x0003
#define RESP_ROTVALVE_ERROR_UNKNOWN              0x00FF
#define RESP_ROTVALVE_MOTOR_BUSY                 0x0004
#define RESP_ROTVALVE_MOTOR_FAILED               0x0005
#define RESP_ROTVALVE_POSITION_UNKNOWN           0x0006
#define RESP_ROTVALVE_TASK_EXECUTING             0x00FE

/* Response code for EZO pump from manual */
#define RESP_EZOPUMP_SUCCESS                    0x0001 
#define RESP_EZOPUMP_ERR_SYNTAX                 0x0002
#define RESP_EZOPUMP_ERR_PENDING                0x00FE // 254
#define RESP_EZOPUMP_ERR_NO_DATA                0x00FF // 255

/* Response code for EZO pump software driver */
#define RESP_EZOPUMP_ERR_PARAMS                 0xE201 // Error in user input parameters
#define RESP_EZOPUMP_ERR_SEND_FAILED            0xE202 // Error in sending command to pump
#define RESP_EZOPUMP_ERR_SEND_OVERFLOW          0xE203 // Too many bytes per command were sent to pump
#define RESP_EZOPUMP_ERR_BUFFER_EMPTY           0xE204 // Missing bytes from response

#endif