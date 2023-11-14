/**
 * Communication protocol for the SAFE system
 * Created by Kevin Tieu, November 12, 2023
*/

#ifndef Comm_h
#define Comm_h

#include "Arduino.h"

#define BUFFER_SIZE 64
#define PARAM_SIZE  56
#define MIN_CMD_LEN 8
#define MIN_RESP_LEN 8
#define MAX_MSG_COUNT 8
#define START_CMD_BITS 0xADDA
#define START_RESP_BITS 0xBEEB
#define STOP_BITS  0xFDDF

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

typedef struct comm_msg_struct {
    uint8_t data[BUFFER_SIZE];
    uint32_t size = 0;
} msg_t;

typedef struct comm_cmd_struct {
    uint8_t cpnt_id;
    uint8_t cmd_id;
    uint32_t parameter[PARAM_SIZE];
    uint8_t parameter_cnt = 0;
} cmd_t;

typedef struct comm_resp_struct {
    uint8_t cpnt_id;
    uint8_t resp_id;
    uint32_t parameter[PARAM_SIZE];
    uint8_t parameter_cnt = 0;
} resp_t;

class CommProtocol {
    private:
    msg_t _buffer_input;
    msg_t _buffer_output;
    cmd_t _command_msg[MAX_MSG_COUNT];
    uint8_t _command_cnt = 0;
    resp_t _response_msg[MAX_MSG_COUNT];
    uint8_t _response_cnt = 0;

    public:
    bool GetBytes();
    bool SendBytes();
    bool StripBytes();
    bool PadBytes();
};



#endif
