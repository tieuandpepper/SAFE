/**
 * 
*/

#include "Arduino.h"
#include "Comm.h"

/// @brief read bytes from serial to received buffer
/// @return true/1 if success
bool CommProtocol::GetBytes()
{
    while (Serial.available() && _buffer_input.size < BUFFER_SIZE)
    {
        _buffer_input.data[_buffer_input.size++] = Serial.read();
    }
    if (_buffer_input.size > 0)
    {
        return true;
    }
    return false;
}


/// @brief write bytes from transmit buffer to serial
/// @return true/1 if success 
bool CommProtocol::CommSendBytes()
{
    if (_buffer_output.size == 0)
    {
        return false;
    }
    Serial.write(_buffer_output.data,_buffer_output.size);
    memset(_buffer_output.data, 0, _buffer_output.size);
    _buffer_output.size = 0;
    return true;
}

/// @brief strip data from incoming bytes
/// @return true/1 if success (there is a correct message)
bool CommProtocol::StripBytes()
{
    uint32_t idx = 0;
    while (idx < _buffer_input.size)
    {
        // search for the start bits ADDA
        if (idx > _buffer_input.size - MIN_CMD_LEN)
        {
            break;
        }
        if (_buffer_input.data[idx++] != (START_CMD_BITS >> 2))
        {
            continue;
        }
        if (_buffer_input.data[idx++] != (START_CMD_BITS & 0x00FF))
        {
            continue;
        }
        // get component ID
        _command_msg[_command_cnt].cpnt_id = _buffer_input.data[idx++];
        // get command ID
        _command_msg[_command_cnt].cmd_id  = _buffer_input.data[idx++];
        // get parameter count, each parameter is 4 bytes long
        _command_msg[_command_cnt].parameter_cnt = _buffer_input.data[idx++];
        
        if (idx > _buffer_input.size - _command_msg[_command_cnt].parameter_cnt - 2)
        {
            break;
        }
        for (uint8_t i = 0; i < _command_msg[_command_cnt].parameter_cnt; i++)
        {
            _command_msg[_command_cnt].parameter[i] =
            (_buffer_input.data[idx]   << 6) | (_buffer_input.data[idx+1] << 4) |
            (_buffer_input.data[idx+2] << 2) | (_buffer_input.data[idx+3]);
            idx += 4;
        }
        // TODO: process CRC
        idx++;
        if (_buffer_input.data[idx++] != (STOP_BITS >> 2))
        {
            continue;
        }
        if (_buffer_input.data[idx++] != (STOP_BITS & 0x00FF))
        {
            continue;
        }
        _command_cnt += 1;
    }

    if (_command_cnt == 0)
    {
        return false;
    }
    return true;
}

bool CommProtocol::PadBytes()
{
    uint32_t idx = _buffer_output.size;
    _buffer_output.data[idx++] = START_RESP_BITS >> 2;
    _buffer_output.data[idx++] = START_RESP_BITS & 0x00FF;
    _buffer_output.data[idx++] = _response_msg[_response_cnt].cpnt_id;
    _buffer_output.data[idx++] = _response_msg[_response_cnt].resp_id;
    _buffer_output.data[idx++] = _response_msg[_response_cnt].parameter_cnt;
    for (uint8_t i = 0; i < _response_msg[_response_cnt].parameter_cnt; i++)
    {
        _buffer_output.data[idx++] = _response_msg[_response_cnt].parameter[i] >> 6;
        _buffer_output.data[idx++] = _response_msg[_response_cnt].parameter[i] >> 4;
        _buffer_output.data[idx++] = _response_msg[_response_cnt].parameter[i] >> 2;
        _buffer_output.data[idx++] = _response_msg[_response_cnt].parameter[i] >> 0;
    }
    _buffer_output.data[idx++] = 0xFF;
    _buffer_output.data[idx++] = STOP_BITS >> 2;
    _buffer_output.data[idx++] = STOP_BITS & 0x00FF;
    _buffer_output.size = idx;
    _response_cnt++;
    return true;
}