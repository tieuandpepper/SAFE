/**
 * @file rotary_valve.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rotary_valve.h"

/**
 * @brief Construct a new Rotary Valve:: Rotary Valve object
 * 
 * @param valve_tx connect to the TX pin of the valve
 * @param valve_rx connect to the RX pin of the valve
 * @param baud_rate: default 9600 bps
 * @note: 8 bits, 1 stop bit, no parity, little endian
 * @attention: It seems like the valve using inverted protocol
 */
RotaryValve::RotaryValve(uint8_t valve_rx, uint8_t valve_tx, uint16_t baud_rate)
            : SoftwareSerial(valve_rx, valve_tx, true)
{
    _baud_rate = baud_rate;
    _rx = valve_rx;
    _tx = valve_tx;
}

/**
 * @brief 
 * 
 */
void RotaryValve::begin_auto()
{
    this->begin(_baud_rate);
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t RotaryValve::ReceiveResponse()
{
    uint8_t response_rx[CMD_LEN_COMMON] = {0};
    uint8_t i = 0;
    if (!this->available())
    {
        return 0xFF;
    }
    while (this->available() && i < CMD_LEN_COMMON)
    {
        response_rx[i++] = this->read();
    }

    _last_response.start_byte = response_rx[0];
    _last_response.address = response_rx[1];
    _last_response.status = response_rx[2];
    _last_response.params[0] = response_rx[3];
    _last_response.params[1] = response_rx[4];
    _last_response.stop_byte = response_rx[5];
    _last_response.check_sum = response_rx[6] + (response_rx[7] << 8);
    if (response_rx[0] != COMM_START_BYTE ||
        response_rx[5] != COMM_STOP_BYTE ||
        i != CMD_LEN_COMMON)
    {
        return 0xFE;
    }
    if (_last_response.check_sum != this->CheckSum(response_rx, RESP_LEN))
    {
        return 0xFD;
    }
    return 1;
}

/**
 * @brief 
 * 
 * @param func_code 
 * @param params 
 * @param cmd_len 
 * @return true 
 * @return false 
 */
bool RotaryValve::SendCommand(uint8_t func_code, uint32_t params, uint8_t cmd_len = CMD_LEN_COMMON)
{
    uint8_t command_tx[CMD_LEN_FACTORY];
    _last_command.address = _address;
    _last_command.cmd_len = cmd_len;
    _last_command.function = func_code;
    _last_command.params[0] = params & 0xFF;
    _last_command.params[1] = (params >> 8) & 0xFF;
    if (cmd_len == CMD_LEN_FACTORY)
    {
        _last_command.params[2] = (params >> 16) & 0xFF;
        _last_command.params[3] = params >> 24;
    }
    if (cmd_len == CMD_LEN_COMMON)
    {
        command_tx[0] = COMM_START_BYTE;
        command_tx[1] = _last_command.address;
        command_tx[2] = _last_command.function;
        command_tx[3] = _last_command.params[0];
        command_tx[4] = _last_command.params[1];
        command_tx[5] = COMM_STOP_BYTE;
        _last_command.check_sum = this->CheckSum(command_tx, CMD_LEN_COMMON);
        command_tx[6] = _last_command.check_sum & 0xFF;
        command_tx[7] = _last_command.check_sum >> 8;
        this->write(command_tx, CMD_LEN_COMMON);
    }
    else {
        command_tx[0] = COMM_START_BYTE;
        command_tx[1] = _last_command.address;
        command_tx[2] = _last_command.function;
        command_tx[3] = (_password >> 24) & 0xFF;
        command_tx[4] = (_password >> 16) & 0xFF;
        command_tx[5] = (_password >> 8)  & 0xFF;
        command_tx[6] = (_password     )  & 0xFF;
        command_tx[7] = _last_command.params[0];
        command_tx[8] = _last_command.params[1];
        command_tx[9] = _last_command.params[2];
        command_tx[10] = _last_command.params[3];
        command_tx[11] = COMM_STOP_BYTE;
        _last_command.check_sum = this->CheckSum(command_tx, CMD_LEN_FACTORY);
        command_tx[12] = _last_command.check_sum & 0xFF;
        command_tx[13] = _last_command.check_sum >> 8;
        this->write(command_tx, CMD_LEN_FACTORY);
    }
    return true;
}

/**
 * @brief 
 * 
 * @return RotValveResp_t 
 */
RotValveResp_t RotaryValve::GetResponse()
{
    return _last_response;
}

/**
 * @brief 
 * 
 * @return RotValveResp_t 
 */
String RotaryValve::PrintResponse()
{
    String response;
    response = ("Address:0x" + String(_last_response.address,HEX));
    response += (" | Status:0x" + String(_last_response.status, HEX));
    response += (" | Params:0x" + String(_last_response.params[0],HEX));
    response += (String(_last_response.params[1],HEX));
    response += (" | CheckSum:0x" + String(_last_response.check_sum, HEX));
    response += (" | Start Byte:0x" + String(_last_response.start_byte, HEX));
    response = response + (" | Stop Byte:0x" + String(_last_response.stop_byte,HEX));
    // response += "\n";
}

/**
 * @brief 
 * 
 * @param destination 
 * @return uint8_t 
 */
uint8_t RotaryValve::ChangePort(uint8_t destination)
{
    uint8_t ret;
    if (destination > 16 || destination < 1)
    {
        return 11;
    }
    if (!this->SendCommand(ACT_FUNC_ROTATE_AUTO, destination << 8))
    {
        return 10;
    }
    delay(1000);
    ret = this->ReceiveResponse();
    return ret;
}

/**
 * @brief 
 * 
 * @param msg 
 * @param msg_length length of the whole message (with checksum)
 * @return uint16_t 
 */
uint16_t RotaryValve::CheckSum(uint8_t msg[], uint8_t msg_length)
{
    uint16_t result = 0;
    // exclude 2 bytes checksum
    msg_length -= 2;
    for (uint8_t i = 0; i < msg_length; ++i)
    {
        result += msg[i];
    }
    return result;
}