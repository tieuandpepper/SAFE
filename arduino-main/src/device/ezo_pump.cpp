/**
 * @file ezo_pump.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "ezo_pump.h"

/**
 * @brief Construct a new EZOPump::EZOPump object
 * 
 * @param addr 
 */
EZOPump::EZOPump(uint8_t addr)
{
    _address_i2c = addr;
}

void EZOPump::Connect()
{
    Wire.begin();
}

/**
 * @brief 
 * 
 * @param command 
 * @return uint16_t 
 */
uint16_t EZOPump::SendCommandSDA(EZOPumpCommand_t command)
{
    char command_buffer[40];
    unsigned int command_length = 0;
    unsigned int byte_written = 0;
    unsigned int i = 0, j = 0;

    for (i = 0; i < command.function.length() && command.function[i] != '\0'; ++i)
    {
        command_buffer[command_length++] = command.function[i];
    }
    for (i = 0; i < command.params_count; ++i)
    {
        command_buffer[command_length++] = ',';
        for (j = 0; j < command.parameters[i].length() && command.parameters[i][j] != '\0';++j)
        {
            command_buffer[command_length++] = command.parameters[i][j];
        }
    }
    command_buffer[command_length] = '\0';
    Wire.beginTransmission(_address_i2c);
    byte_written = Wire.write(command_buffer);
    Wire.endTransmission();
    _last_command = command;
    _last_command.time_sent = millis();
    // Serial.println(byte_written);
    // Serial.println(command_length);
    // Serial.println(command_buffer);
    if (byte_written < command_length)
    {
        return RESP_EZOPUMP_ERR_SEND_FAILED;
    }
    if (byte_written > command_length)
    {
        return RESP_EZOPUMP_ERR_SEND_OVERFLOW;
    }
    return RESP_GENERAL_FEEDBACK_SUCCESS;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t EZOPump::GetResponseSDA()
{   
    uint8_t in_char = 0;
    uint8_t buffer_length = 0;
    char buffer[40];
    while (millis() - _last_command.time_sent < DEFAULT_PROCESSING_TIME)
    {
        delay(10);
    }
    Wire.requestFrom(_address_i2c, 20, 1);
    _last_response.status = Wire.read();

    while (Wire.available()) {          //are there bytes to receive.
        in_char = Wire.read();            //receive a byte.
        buffer[buffer_length++] = in_char;            //load this byte into our array.
        if (in_char == 0) {               //if we see that we have been sent a null command.
            break;                          //exit the while loop.
        }
    }
    if (buffer_length > 0)
    {
        _last_response.parameter = String(buffer);
        _last_response.returned_params = true;
    }
    else {
        _last_response.returned_params = false;
    }
    return _last_response.status;
}

/**
 * @brief 
 * 
 * @param volume 
 * @return uint16_t 
 */
uint16_t EZOPump::SingleReport(String * volume)
{
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    EZOPumpCommand_t cmd;
    cmd.function = CMD_SINGLE_READING;
    cmd.params_count = 0;
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    (*volume) = _last_response.parameter;
    return res;
}

/**
 * @brief 
 * 
 * @param reverse 
 * @return uint16_t 
 */
uint16_t EZOPump::DispenseContinuous(bool reverse)
{
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    EZOPumpCommand_t cmd;
    cmd.function = CMD_DISPENSE_MODE;
    cmd.params_count = 1;
    if (reverse)
    {
        cmd.parameters[0] = '-*';
    }
    else {
        cmd.parameters[0] = '*';
    }

    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    return res;
}

/**
 * @brief 
 * 
 * @param volume 
 * @return uint16_t 
 */
uint16_t EZOPump::DispenseVolume(float volume)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_DISPENSE_MODE;
    cmd.params_count = 1;
    cmd.parameters[0] = String(volume);
    res = this->SendCommandSDA(cmd);
    // Serial.print("send status:");Serial.println(res,HEX);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    // Serial.print("Get status:");Serial.println(res,HEX);
    return res;   
}

/**
 * @brief 
 * 
 * @param response 
 * @return uint16_t 
 */
uint16_t EZOPump::DispenseStatus(String * response)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_DISPENSE_MODE;
    cmd.params_count = 1;
    cmd.parameters[0] = '?';
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    (*response) = _last_response.parameter;
    return res;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t EZOPump::DispensePause()
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_PAUSE_DISPENSE;
    cmd.params_count = 0;
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    return res;   
}

/**
 * @brief 
 * 
 * @param response 
 * @return uint16_t 
 */
uint16_t EZOPump::DispenseStop(String * response)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_STOP_DISPENSE;
    cmd.params_count = 0;
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    (*response) = _last_response.parameter;
    return res;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t EZOPump::DirectionChange()
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_INVERT_DIRECTION;
    cmd.params_count = 0;
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    return res;
}

/**
 * @brief 
 * 
 * @param correct_vol_ml 
 * @return uint16_t 
 */
uint16_t EZOPump::Calibrate(float correct_vol_ml)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_CALIBRATE;
    cmd.params_count = 1;
    cmd.parameters[0] = String(correct_vol_ml);
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    return res;   
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t EZOPump::CalibrateReset()
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_DISPENSE_MODE;
    cmd.params_count = 1;
    cmd.parameters[0] = "clear";
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    return res;
}

/**
 * @brief 
 * 
 * @param voltage 
 * @return uint16_t 
 */
uint16_t EZOPump::VoltageCheck(String * voltage)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_PUMP_VOLTAGE;
    cmd.params_count = 1;
    cmd.parameters[0] = "?";
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    (*voltage) = _last_response.parameter;
    return res;   
}

/**
 * @brief 
 * 
 * @param info 
 * @return uint16_t 
 */
uint16_t EZOPump::DeviceInfo(String * info)
{
    EZOPumpCommand_t cmd;
    uint16_t res = RESP_EZOPUMP_ERR_SEND_FAILED;
    cmd.function = CMD_DEVICE_INFO;
    cmd.params_count = 0;
    res = this->SendCommandSDA(cmd);
    if (res != RESP_GENERAL_FEEDBACK_SUCCESS)
    {
        return res;
    }
    res = this->GetResponseSDA();
    if (!_last_response.returned_params)
    {
        return RESP_EZOPUMP_ERR_BUFFER_EMPTY;
    }
    (*info) = _last_response.parameter;
    return res;  
}