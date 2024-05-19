/**
 * @file ezo_pump.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef EZO_PUMP_H
#define EZO_PUMP_H

#include <Arduino.h>
#include <Wire.h>                //enable I2C.
#include "response_error_code.h"

// Communication mode
#define COMM_MODE_UART        0xAA
#define COMM_MODE_I2C         0x2C

// I2C addresses format
#define I2C_ADDR_DEFAULT      0x67
#define I2C_ADDR_MIN          0x01
#define I2C_ADDR_MAX          0x7F

// delay time
#define DEFAULT_PROCESSING_TIME  300   // 300ms

// Command
#define CMD_SWITCH_UART           "Baud"
#define CMD_CALIBRATE             "Cal"
#define CMD_FAC_RESET             "Factory"
#define CMD_DISPENSE_MODE         "D"
#define CMD_PAUSE_DISPENSE        "P"
#define CMD_STOP_DISPENSE         "X"
#define CMD_TOTAL_VOL_DISP        "Tv"
#define CMD_INVERT_DIRECTION      "Invert"
#define CMD_DEVICE_INFO           "i"
#define CMD_STATUS_INFO           "Status"
#define CMD_FIND_DEVICE           "Find"
#define CMD_CHANGE_ADDR           "I2C"
#define CMD_LED_CONTROL           "L"
#define CMD_DEVICE_NAME           "Name"
#define CMD_PARAMS                "O"
#define CMD_PROTOCOL_LOCK         "Plock"
#define CMD_PUMP_VOLTAGE          "Pv"
#define CMD_SINGLE_READING        "R"
#define CMD_SLEEP_LOW_POWER       "Sleep"

typedef struct EZOPump_Cmd_Type {
    String function;
    String parameters[4];
    uint8_t params_count = 0;
    uint32_t time_sent;
} EZOPumpCommand_t;

typedef struct EZOPump_Resp_Type {
    uint8_t status;
    // String function;
    String parameter;
    bool returned_params = false;
} EZOPumpResponse_t;

class EZOPump {
    private:
        uint8_t _address_i2c;
        uint8_t _comm_mode;
        EZOPumpCommand_t _last_command;
        EZOPumpResponse_t _last_response;
    public:
        EZOPump(uint8_t addr = I2C_ADDR_DEFAULT);
        void Connect();
        uint16_t SendCommandSDA(EZOPumpCommand_t command);
        uint16_t GetResponseSDA();
        // uint16_t GetLastReponse();
        uint16_t SingleReport(String * volume);
        uint16_t DispenseContinuous(bool reverse);
        uint16_t DispenseVolume(float volume);
        uint16_t DispenseStatus(String * response);
        uint16_t DispensePause();
        uint16_t DispenseStop(String * response);
        uint16_t DirectionChange();
        uint16_t Calibrate(float correct_vol_ml);
        uint16_t CalibrateReset();
        uint16_t VoltageCheck(String * voltage);
        uint16_t DeviceInfo(String * info);

};

#endif