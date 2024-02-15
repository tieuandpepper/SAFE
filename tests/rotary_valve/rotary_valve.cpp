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
 */
// RotaryValve::RotaryValve(uint8_t valve_tx, uint8_t valve_rx, uint16_t baud_rate)
// {
//     _baud_rate = baud_rate;
//     _rx = valve_rx;
//     _tx = valve_tx;
//     // _valve_obj = SoftwareSerial(valve_rx, valve_tx, true);
// }

// void RotaryValve::Connect(void)
// {
//     _valve_obj.begin(_baud_rate);
// }

// void RotaryValve::SetUp(void)
// {

// }



/**
 * @brief return cumulative checksum from header to end of frame
 * 
 * @param cmd_type : factory or common
 * @param func_code 
 * @param params 
 * @return uint16_t 
 */
static uint16_t CheckSum(uint8_t cmd_type, uint8_t func_code, uint32_t params)
{
    uint16_t result = COMM_START_BYTE + COMM_STOP_BYTE;
    result += func_code;
    if (cmd_type & COMM_TYPE_FACTORY)
    {
        result += SETTING_PASSWORD;
    }
    uint8_t i = 0;
    for (i=0; i < 4; i++)
    {
        result += (params >> (i*8)) & 0xFF;
    }
    return result;
}