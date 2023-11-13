/**
 * Communication protocol for the SAFE system
 * Created by Kevin Tieu, November 12, 2023
*/

#ifndef Comm_h
#define Comm_h

#include "Arduino.h"

#define BUFFER_SIZE 64
#define START_BITS 0xADDA
#define STOP_BITS  0xBCCB

// Response ID

#define RESP_ACK 0x1A
#define RESP_NACK 0x9F
#define RESP_DATA 0x2D

// Component ID
#define ARDUINO_CPNT_ID 0xA1
#define PUMP_1_CPNT_ID 0xB1
#define PUMP_2_CPNT_ID 0xB2

// Command ID
enum pump_cmd_id
{
    PUMP_CMD_START = 0x11,
    PUMP_CMD_STOP,
    PUMP_CMD_DISPENSE,
    PUMP_CMD_TOGGLE_DIR,
    PUMP_CMD_SET_DIR,
    PUMP_CMD_RUN_MS,
    PUMP_CMD_SET_MAX_VOLT,
    PUMP_CMD_SET_MIN_VOLT,
    PUMP_CMD_SET_MAX_FLOW,
    PUMP_CMD_SET_MIN_FLOW = 0x20,
    PUMP_CMD_SET_SPEED,
    PUMP_CMD_PRIME_START,
    PUMP_CMD_PRIME_STOP,
    PUMP_CMD_PRIME_MS,
};

typedef struct comm_buffer_struct {
    uint8_t data[BUFFER_SIZE];
    uint32_t size = 0;
} comm_buffer_t;

bool CommGetBytes(comm_buffer_t* comm_input_buffer);
bool CommSendBytes(comm_buffer_t* comm_output_buffer);


#endif
