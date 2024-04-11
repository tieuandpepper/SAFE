/**
 * @file response_code.h
 * @author your name (you@domain.com)
 * @brief define response value for each driver. Format shoud be 0xHHHH (2 bytes)
 *        DRIVER FEEDBACK (NORMAL OPERATION): 0xDF__
 *        DRIVER ERROR                      : 0xDE__
 *        DEVICE FEEDBACK                   : 0x__
 *        DEVICE ERRROR                     : 0x__
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RESPONSE_CODE_H
#define RESPONSE_CODE_H

/* Universal 0xDF0_ or OxDE0_ */ 
#define RESP_FEEDBACK_VOID                      0xDF00 //nothing to return
#define RESP_FEEDBACK_SUCCESS                   0xDF01
#define RESP_FEEDBACK_ERR_INVALID               0xDE02

/* TEMPERATURE SENSOR */
#define RESP_TEMPSENSOR_READ_DURATION_DONE      0xDF11
#define RESP_TEMPSENSOR_READ_PERIOD_WAITING     0xDF12
#define RESP_TEMPSENSOR_ERR_SENSOR              0xDE13

/* Response from ROTARY VALVE software driver */
// Error in INPUT parameters
#define RESP_ROTVALVE_ERR_PARAMS                 0xDEE1
// Error in connecting to the valve (no response)
#define RESP_ROTVALVE_ERR_CONNECTION             0xDEE2
// Error in sending command
#define RESP_ROTVALVE_ERR_SEND_FAILED            0xDEE3
#define RESP_ROTVALVE_ERR_SEND_OVERFLOW          0xDEE4
// Error in receiving response
#define RESP_ROTVALVE_ERR_BUFFER_EMPTY           0xDEE5
#define RESP_ROTVALVE_ERR_BUFFER_MISSING         0xDEE6
#define RESP_ROTVALVE_ERR_FRAME                  0xDEE7
#define RESP_ROTVALVE_ERR_CHECKSUM               0xDEE8
#define RESP_ROTVALVE_ERR_INVALID                0xDEE9


#endif