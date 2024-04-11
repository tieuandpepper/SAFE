/**
 * @file rotary_valve.cpp
 * @author Kevin Tieu
 * @brief driver for Aurora ERV 16-port rotary valve for Arduino
 * @version 0.2
 * @date 2024-03-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rotary_valve.h"

/**
 ************************************************************************************************
 * Public methods
 ************************************************************************************************
 */

/**
 **********************************
 * constructor and initialization *
 **********************************
 */

/**
 * @brief Constructor
 * 
 * @param valve_tx connect to the TX pin of the valve
 * @param valve_rx connect to the RX pin of the valve
 * @param baud_rate: default 9600 bps
 * @note: 8 bits, 1 stop bit, no parity, little endian
 * @attention: It seems like the valve using inverted protocol (idle at 0V)
 */
RotaryValve::RotaryValve(uint8_t valve_rx, uint8_t valve_tx, int port_count, uint16_t baud_rate)
            : SoftwareSerial(valve_rx, valve_tx, true)
{
    this->_baud_rate_code = GetBaudRateEncoding(baud_rate);
    this->_rx = valve_rx;
    this->_tx = valve_tx;
    this->_port_count = (uint8_t)port_count;
}

/**
 * @brief connect to valve, can perform some intializing processes
 * @details: get current version, 
 */
void RotaryValve::Initialize()
{
    this->begin(baud_rate_encode_arr[this->_baud_rate_code]);
}

/**
 ********************************
 * low-level hardware interface *
 ********************************
 */

/**
 * @brief Send command by TX line - RS232/485
 * 
 * @param func_code function code for byte 2 (B2) 
 * @param params parameters for commands
 * @param cmd_len command length, in bytes. Either 8 or 14. Default is 8 bytes
 * @struct @common <HEADER><ADDR><FUNC><PARAMS[0]><PARAMS[1]><EOF><CSUM_LOW><CSUM_HIGH>
 * @struct @factory <HEADER><ADDR><FUNC><PASSWORD[0-3]><PARAMS[0-3]><EOF><CSUM_LOW><CSUM_HIGH>
 * @return success/error code 
 */
int RotaryValve::SendCommandTX(uint8_t func_code, uint32_t params, uint8_t cmd_len = CMD_LEN_COMMON)
{
    uint8_t command_tx[CMD_LEN_FACTORY];
    unsigned int byte_overwritten = 0;
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
        byte_overwritten = this->write(command_tx, CMD_LEN_COMMON) - CMD_LEN_COMMON;
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
        byte_overwritten = this->write(command_tx, CMD_LEN_FACTORY) - CMD_LEN_FACTORY;
    }
    if (byte_overwritten < 0)
    {
        return RESP_ROTVALVE_ERR_SEND_FAILED;
    }
    if (byte_overwritten > 0)
    {
        return RESP_ROTVALVE_ERR_SEND_OVERFLOW;
    }
    return RESP_FEEDBACK_SUCCESS;
}

/**
 * @brief read 10 times (every 100ms) for 1s until response is received.
 *        read the serial buffer and decode response
 *        save response to _last_response struct for future access
 * 
 * @return int error code
 *  success: 0x00
 *  failure from valve: 0x01-0x06, 0xFE, 0xFF
 *  failure from Arduino/driver: 0xE1 - 0xEF
 */
int RotaryValve::GetResponseRX()
{
    uint8_t response_rx[CMD_LEN_COMMON] = {0};
    uint8_t read_count = 10;
    bool data_available = false;
    uint8_t i = 0;
    // wait for response to arrive
    while (read_count-- > 0)
    {
        if (this->available())
        {
            data_available = true;
            break;
        }
        delay(50);
    }

    // unavailable data in buffer
    if (!data_available)
    {
        return RESP_ROTVALVE_ERR_BUFFER_EMPTY;
    }
    //  read 8 bytes
    while (this->available() && i < CMD_LEN_COMMON)
    {
        response_rx[i++] = this->read();
    }
    // missing bytes
    if (i < CMD_LEN_COMMON)
    {
        return RESP_ROTVALVE_ERR_BUFFER_MISSING;
    }
    //  message frame does not start with frame header or end with EOF
    if (response_rx[0] != COMM_START_BYTE ||
        response_rx[5] != COMM_STOP_BYTE )
    {
        return RESP_ROTVALVE_ERR_FRAME;
    }
    
    // copy response message into response struct
    _last_response.start_byte = response_rx[0];
    _last_response.address = response_rx[1];
    _last_response.status = response_rx[2];
    _last_response.params[0] = response_rx[3];
    _last_response.params[1] = response_rx[4];
    _last_response.stop_byte = response_rx[5];
    _last_response.check_sum = response_rx[6] + (response_rx[7] << 8);

    // inspect checksum
    if (_last_response.check_sum != this->CheckSum(response_rx, RESP_LEN))
    {
        return RESP_ROTVALVE_ERR_CHECKSUM;
    }
    return RESP_FEEDBACK_SUCCESS;
}

/**
 *********************************
 * high-level software interface *
 *********************************
 */
/**
 ----------------------------
 | factory setting commands |
 ----------------------------
 */

/**
 * @brief Set address for current device
 * @param addr value for B7
 * B7=0xXX (B8=0x00 B9=0x00 B10=0x00)
 * The value range of XX is 00~7F in V1.9 & above version.
 * 00~FF in version below V1.9, the default is 00.
 * @return int success/error code
 */
int RotaryValve::FactorySetAddr(uint32_t addr)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // the version is 0x0109 (V1.9) and above
    if (this->_version > 0x0109 && addr > 0x7F)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    // Send command
    res = this->SendCommandTX(SETTING_FUNC_SET_ADDRESS, addr, CMD_LEN_FACTORY);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    // successfully change addresss, update address in class
    this->_address = addr;
    return res;
}

/**
 * @brief Set baud rate for RS232 protocol
 * @ default 9600 bps
 * @param baud_rate 9600, 19200, 38400, 57600, 115200 bps
 * @return int success/error code
 */
int RotaryValve::FactorySetBaudRate(uint32_t baud_rate)
{
    uint8_t baud_rate_code = GetBaudRateEncoding(baud_rate);
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    if (baud_rate_code == baud_rate_encode_arr_len)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    // send command
    res = this->SendCommandTX(SETTING_FUNC_SET_SPEED_RS232, baud_rate_code, CMD_LEN_FACTORY);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Send command successfully
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    // successfully change addresss, update baud rate in class
    this->_baud_rate_code = baud_rate_code;
    return res;
}

/**
 * @brief set automatic reset when power on
 * Automatically reset to the position between port 1 and the highest port after power on
 * @default automatically reset
 * @param power_on_reset 
 * 0x00: non-automatic reset
 * 0x01: automatic reset
 * @return int success/error code
 */
int RotaryValve::FactorySetAutoReset(uint8_t power_on_reset)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // paramater validation
    if (power_on_reset != SETTING_PARAMS_AUTO_RESET_ON &&
        power_on_reset != SETTING_PARAMS_AUTO_RESET_OFF)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    // Send command
    res = this->SendCommandTX(SETTING_FUNC_SET_AUTO_RESET, power_on_reset, CMD_LEN_FACTORY);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief restore factory setting
 * 
 * @return int success/error code
 */
int RotaryValve::FactoryReset()
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(SETTING_FUNC_FACTORY_SETTING, 0x0000, CMD_LEN_FACTORY);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 ------------------------
 | query (get) commands |
 ------------------------
 */

/**
 * @brief get address of current device
 * Address range from 0x0000 to 0x007F
 * Default: 0x00
 * @return int success/error code/ status
 */
int RotaryValve::QueryAddress(int * addr)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(QUERY_FUNC_ADDRESS);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    (*addr) = (this->_last_response.params[1] << 8) | this->_last_response.params[0];
    return res;
}

/**
 * @brief query RS232 baud rate
 * 
 * @param baud_rate returned baud_rate
 *        0x0000: 9600 bps
 *        0x0001: 19200 bps
 *        0x0002: 38400 bps
 *        0x0003: 57600 bps
 *        0x0004: 115200 bps
 * @return int success/error code/status
 */
int RotaryValve::QueryBaudRate(int * baud_rate)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(QUERY_FUNC_RS232_SPEED);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    if (this->_last_response.params[1] >= baud_rate_encode_arr_len)
    {
        return RESP_ROTVALVE_ERR_INVALID;
    }
    (*baud_rate) = baud_rate_encode_arr[this->_last_response.params[1]];
    return res;
}

/**
 * @brief Get motor status
 * 
 * @param status status code
 * @return int error code/status
 */
int RotaryValve::QueryMotorStatus(int * status)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(QUERY_FUNC_MOTOR_STATUS);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status == RESP_VALVE_ERROR_FRAME ||
        this->_last_response.status == RESP_VALVE_ERROR_PARAMS ||
        this->_last_response.status == RESP_VALVE_ERROR_OPTO)
    {
        return this->_last_response.status;
    }
    (*status) = this->_last_response.status;
    return res;
}

/**
 * @brief Get current version
 * 
 * @param version pointer value
 * @return int error code/status
 */
int RotaryValve::QueryCurrVersion(int * version)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(QUERY_FUNC_CURRENT_VERSION);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    (*version) = (this->_last_response.params[0] << 8) | this->_last_response.params[1];
    return res;
}

/**
 * @brief Query automatic reset when power on
 * 
 * @param power_on_reset pointer value
 * @return int error code/status
 */
int RotaryValve::QueryAutoReset(int * power_on_reset)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(QUERY_FUNC_AUTO_RESET);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    (*power_on_reset) = (this->_last_response.params[0] << 8) | this->_last_response.params[1];
    return res;
}

/**
 -------------------------
 | Action commands (Set) |
 -------------------------
 */

/**
 * @brief The selector valve runs to the reset optocoupler and stops
 * 
 * @return int error/status code
 */
int RotaryValve::ActionReset()
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(ACT_FUNC_RESET);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief The selector valve runs to the encoder origin position
 *        Overlapping with reset position of RESET (0x45) command
 * 
 * @return int error/status code
 */
int RotaryValve::ActionResetOrigin()
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(ACT_FUNC_ORIGIN_RESET);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief the motor rotates through the code disc
 *        select the best path automatically
 *         
 * @param port 1 - highest port number
 * @return int error/status code
 */
int RotaryValve::ActionMoveAuto(uint8_t port)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // validate port number
    if (port < this->_port_first || port > this->_port_count)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    // Send command
    res = this->SendCommandTX(ACT_FUNC_ROTATE_AUTO, port);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief Switch port according to the required direction
 * According to the actual number of ports of the selector valve, 
 * the parameter cannot exceed the maximum number of ports of the current valve, 
 * and B3, B4 must be two adjacent ones.
 * Example:
 * The current valve connected port: 1
 * the target: run counterclockwise to port 4
 * -> Command: 0xA4 Parameter: 0x0304
 * The target: run clockwise to port 4
 * -> Command: 0xA4 Parameter: 0x0504
 * @param port 
 * @param direction 
 * @return int 
 */
int RotaryValve::ActionMoveWithDir(uint8_t port, uint8_t direction)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    uint8_t adjacent_port;
    // find adjacent_port
    if (direction == DIRECTION_CLOCKWISE)
    {
        adjacent_port = port + 1;
        // adjacent_port is higher than the highest port number
        if (adjacent_port > this->_port_count)
        {
            adjacent_port = this->_port_first;
        }
    }
    else {
        adjacent_port = port - 1;
        // adjacent port is lower than the first port (1)
        if (adjacent_port < this->_port_first)
        {
            adjacent_port = this->_port_count;
        }
    }
    
    // Send command
    res = this->SendCommandTX(ACT_FUNC_SWITCH_PORT_IN_DIR, (adjacent_port << 8) | port);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief Switch between ports (in the middle between 2 adjacent ports) in the required direction
 * According to the actual number of ports of the selector valve, 
 * the parameter cannot exceed the maximum number of ports of the current valve, 
 * and B3, B4 must be two adjacent ones.
 * Example
 * The current valve connected port is 1
 * The target: run counterclockwise to between port 3 and port 4
 * -> Command: 0xB4, Parameter: 0x0304
 * The valve will run counterclockwise through port 3 and stop between port 3 and port 4.
 * @param port1 the first port
 * @param port2 the second port
 * The valve stops between @b port1 and @b port2
 * @return int 
 */
int RotaryValve::ActionMoveBetween(uint8_t port1, uint8_t port2)
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // parameter processing
    if (port1 < this->_port_first || port1 > this->_port_count)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    if (port2 < this->_port_first || port2 > this->_port_count)
    {
        return RESP_ROTVALVE_ERR_PARAMS;
    }
    
    // Send command
    res = this->SendCommandTX(ACT_FUNC_SWITCH_BETWEEN_PORT_IN_DIR, (port1 << 8) | port2);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 * @brief Stop forcibly
 * 
 * @return int error/status code
 */
int RotaryValve::ActionStop()
{
    int res = RESP_ROTVALVE_ERR_SEND_FAILED;
    // Send command
    res = this->SendCommandTX(ACT_FUNC_FORCE_STOP);
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Get response
    res = this->GetResponseRX();
    if (res != RESP_FEEDBACK_SUCCESS)
    {
        return res;
    }
    // Success getting a response, check for response status
    if (this->_last_response.status != RESP_VALVE_NORMAL_STATUS)
    {
        return this->_last_response.status;
    }
    return res;
}

/**
 ************************************************************************************************
 * Private methods
 ************************************************************************************************
 */

/**
 * @brief 
 * 
 * @param msg byte array containing the message (length is 8 or 14)
 * @param msg_length length of the whole message (with checksum), default 8
 * @return uint16_t checksum value (0xAABB), where 0xBB is the high byte, 0xAA is the low byte
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

/**
 ************************************************************************************************
 * Static helper methods
 ************************************************************************************************
 */

/**
 * @brief return baud_rate code for given baud rate
 * 
 * @param baud_rate 9600, 19200, 38400, 57600, 115200
 * @return uint8_t (0-4)
 * @error return 5
 */
static uint8_t GetBaudRateEncoding(uint16_t baud_rate)
{
    uint8_t i = 0;
    // find baud rate from encoding array
    while (i++ < baud_rate_encode_arr_len)
    {
        if (baud_rate_encode_arr[i] == baud_rate)
        {
            break;
        }
    }
    return i;
}