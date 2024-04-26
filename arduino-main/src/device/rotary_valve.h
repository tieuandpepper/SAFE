/**
 * @file rotary_valve.h
 * @author Kevin Tieu (phattieuthien@gmail.com)
 * @brief Driver for Aurora Electrical Rotary Valve 16 ports (ASP-ERV-O1.0-16)
 *        Orifice Size: 1mm, Max pressure 1.5MPa, Wetted materials: PCTFE/Sapphire
 * @note This driver is for RS232/RS485
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ROTARY_VALVE_H
#define ROTARY_VALVE_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "response_error_code.h"

/* Communication properties */
#define COMM_DATA_BITS    8
#define COMM_PARITY_BITS  0
#define COMM_CSUM_BITs    2


// common command, response
#define CMD_LEN_COMMON        8
#define RESP_LEN              8
// factory command
#define CMD_LEN_FACTORY      14

/* Command/reponse macros */
#define COMM_START_BYTE        0xCC
#define COMM_STOP_BYTE         0xDD

/* Factory command function code */
#define SETTING_FUNC_SET_ADDRESS            0x00
#define SETTING_FUNC_SET_SPEED_RS232        0x01
#define SETTING_FUNC_SET_SPEED_RS485        0x02
#define SETTING_FUNC_SET_SPEED_CAN          0x03
#define SETTING_FUNC_SET_AUTO_RESET         0x0E
#define SETTING_FUNC_SET_ADDRESS_CAN        0x10
#define SETTING_FUNC_SET_ADDRESS_CH_1_MULI  0x50
#define SETTING_FUNC_SET_ADDRESS_CH_2_MULI  0x51
#define SETTING_FUNC_SET_ADDRESS_CH_3_MULI  0x52
#define SETTING_FUNC_SET_ADDRESS_CH_4_MULI  0x53
#define SETTING_FUNC_PARAMS_LOCK            0xFC
#define SETTING_FUNC_FACTORY_SETTING        0xFF

/* Factory setting command */
#define SETTING_PASSWORD                     0xFFEEBBAA // B3=0xFF, B6 = 0xAA
// Autmatic reset: reset to the position between port 1 and the port with the highest number
// Default: ON
#define SETTING_PARAMS_AUTO_RESET_ON         0x01
#define SETTING_PARAMS_AUTO_RESET_OFF        0x00
// Lock paramenter
#define SETTING_PARAMS_LOCK                  0x00
// Restore factory setting
#define SETTING_PARAMS_FACTORY_RESET         0x00

/* Query Command function code */
#define QUERY_FUNC_ADDRESS                   0x20
#define QUERY_FUNC_RS232_SPEED               0x21   
#define QUERY_FUNC_RS485_SPEED               0x22   
#define QUERY_FUNC_CAN_SPEED                 0x23   
#define QUERY_FUNC_AUTO_RESET                0x2E   
#define QUERY_FUNC_ADDRESS_CAN               0x30
#define QUERY_FUNC_MULTI_CH_1_ADDR           0x70   
#define QUERY_FUNC_MULTI_CH_2_ADDR           0x71   
#define QUERY_FUNC_MULTI_CH_3_ADDR           0x72   
#define QUERY_FUNC_MULTI_CH_4_ADDR           0x73   
#define QUERY_FUNC_CURRENT_POSITION          0x3E   
#define QUERY_FUNC_CURRENT_VERSION           0x3F
#define QUERY_FUNC_MOTOR_STATUS              0x4A

/* Action command function code */
#define ACT_FUNC_ROTATE_AUTO                 0x44 // get to the desired port automatically
#define ACT_FUNC_RESET                       0x45
#define ACT_FUNC_ORIGIN_RESET                0x4F // run to the origin position
#define ACT_FUNC_SWITCH_PORT_IN_DIR          0xA4
#define ACT_FUNC_SWITCH_BETWEEN_PORT_IN_DIR  0xB4
#define ACT_FUNC_FORCE_STOP                  0x49

/* Action command parameter for driver */
#define DIRECTION_CLOCKWISE                  0x01
#define DIRECTION_COUNTERCLOCKWISE           0x02

static uint16_t baud_rate_encode_arr[] = {9600, 19200, 38400, 57600, 115200};
static uint8_t baud_rate_encode_arr_len = sizeof(baud_rate_encode_arr) / sizeof(uint16_t);

typedef struct RotaryValveCmd {
    uint8_t cmd_len = CMD_LEN_COMMON;
    uint8_t function;
    uint8_t address;
    uint8_t params[4] = {0};
    uint16_t check_sum;
} RotValvecommand_t;

typedef struct RotaryValveResp {
    uint8_t resp_len = RESP_LEN;
    uint8_t status;
    uint8_t address;
    uint8_t params[2] = {0};
    uint16_t check_sum;
    uint8_t start_byte;
    uint8_t stop_byte;
    
} RotValverespond_t;

class RotaryValve : public SoftwareSerial {
    private:
        uint8_t _rx;
        uint8_t _tx;
        uint16_t _baud_rate_code;
        uint16_t _version = 0;
        uint8_t _address = 0;
        uint32_t _password = SETTING_PASSWORD;
        uint8_t _port_count = 16;
        uint8_t _port_first = 1;
        uint8_t _home_port = 1;
        uint8_t _current_port;
        RotValvecommand_t _last_command;
        RotValverespond_t _last_response;
        // helper
        uint16_t CheckSum(uint8_t msg[], uint8_t msg_length = CMD_LEN_COMMON);
    public:
        // constructor and initialization
        RotaryValve(uint8_t valve_tx, uint8_t valve_rx, uint8_t port_count, uint16_t baud_rate = 9600);
        void Connect();
        // low-level interface with hardware
        uint16_t SendCommandTX(uint8_t func_code, uint32_t params = 0, uint8_t cmd_len = CMD_LEN_COMMON);
        uint16_t GetResponseRX();
        // high-level interface
        // factory setting command
        uint16_t FactorySetAddr(uint32_t addr);
        uint16_t FactorySetBaudRate(uint32_t baud_rate);
        uint16_t FactorySetAutoReset(uint8_t power_on_reset);
        uint16_t FactoryReset();
        // Query command (Get commands)
        uint16_t QueryAddress(uint32_t * addr);
        uint16_t QueryBaudRate(uint32_t * baud_rate);
        uint16_t QueryMotorStatus(uint32_t * status);
        uint16_t QueryCurrVersion(uint32_t * version);
        uint16_t QueryAutoReset(uint32_t * power_on_reset);
        // Action command
        uint16_t ActionReset();
        uint16_t ActionResetOrigin();
        uint16_t ActionMoveAuto(uint8_t port);
        uint16_t ActionMoveWithDir(uint8_t port, uint8_t direction = DIRECTION_CLOCKWISE);
        uint16_t ActionMoveBetween(uint8_t port1, uint8_t port2);
        uint16_t ActionStop();
};

// static helpers
static uint8_t GetBaudRateEncoding(uint16_t baud_rate);

#endif