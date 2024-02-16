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

/* Physical properties */
#define PORTS_COUNT      16
#define PORT_ID_MAX      0x0F


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
#define SETTING_PASSWORD     0xFFEEBBAA // B3=0xFF, B6 = 0xAA
#define SETTING_PARAMS_PADDING               0x00000000 // padding for B7-B10
#define SETTING_PARAMS_BAUD_RATE_9600        0x00
#define SETTING_PARAMS_BAUD_RATE_19200       0x01
#define SETTING_PARAMS_BAUD_RATE_38400       0x02
#define SETTING_PARAMS_BAUD_RATE_57600       0x03
#define SETTING_PARAMS_BAUD_RATE_115200      0x04
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
#define ACT_FUNC_ORIGIN_SET                  0x4F // run to the origin position
#define ACT_FUNC_SWITCH_PORT_IN_DIR          0xA4
#define ACT_FUNC_SWITCH_PORTS_IN_DIR         0xB4
#define ACT_FUNC_FORCE_STOP                  0x49

/* Action and Query Params B3 B4 */
#define QUERY_PARAMS_PADDING                 0x0000
#define ACT_PARAMS_PADDING                   0x0000

/* Response function code B2 */
#define RESP_STATUS_NORMAL_STATUS              0x00
#define RESP_STATUS_FRAME_ERROR                0x01
#define RESP_STATUS_PARAMS_ERROR               0x02
#define RESP_STATUS_OPTO_ERROR                 0x03
#define RESP_STATUS_MOTOR_BUSY                 0x04
#define RESP_STATUS_MOTOR_FAILED               0x05
#define RESP_STATUS_UNKNOWN_POSITION           0x06
#define RESP_STATUS_TASK_EXECUTING             0xFE
#define RESP_STATUS_UNKNOWN_ERROR              0xFF


enum PORT_ID {
    PORT_ID_1 = 0x01,
    PORT_ID_2,
    PORT_ID_3,
    PORT_ID_4,
    PORT_ID_5,
    PORT_ID_6,
    PORT_ID_7,
    PORT_ID_8,
    PORT_ID_9,
    PORT_ID_10,
    PORT_ID_11,
    PORT_ID_12,
    PORT_ID_13,
    PORT_ID_14,
    PORT_ID_15,
    PORT_ID_16,
};

typedef struct RotaryValveCmd {
    uint8_t cmd_len = CMD_LEN_COMMON;
    uint8_t function;
    uint8_t address;
    uint8_t params[4] = {0};
    uint16_t check_sum;
    uint32_t password;
} RotValveCmd_t;

typedef struct RotaryValveResp {
    uint8_t resp_len = RESP_LEN;
    uint8_t status;
    uint8_t address;
    uint8_t params[2] = {0};
    uint16_t check_sum;
    uint8_t start_byte;
    uint8_t stop_byte;
} RotValveResp_t;

class RotaryValve : public SoftwareSerial {
    private:
        uint8_t _rx;
        uint8_t _tx;
        uint16_t _baud_rate;
        uint8_t _address = 0;
        uint32_t _password = SETTING_PASSWORD;
        uint8_t _home_port;
        uint8_t _current_port;
        RotValveCmd_t _last_command;
        RotValveResp_t _last_response;
        // helper
        uint16_t CheckSum(byte msg[], uint8_t msg_length);
    public:
        RotaryValve(uint8_t valve_tx, uint8_t valve_rx, uint16_t baud_rate = 9600);
        void begin_auto();
        int ReceiveResponse();
        bool SendCommand(uint8_t func_code, uint32_t params = 0, uint8_t cmd_len = CMD_LEN_COMMON);
        RotValveResp_t GetResponse();
        String PrintResponse();

};



#endif