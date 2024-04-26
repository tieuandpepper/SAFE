#include "pump_masterflex.h"

/**
 * @brief Construct a new Pump Masterflex:: Pump Masterflex object
 * 
 * @param bd25 contains a struct of input/output pins on Arduino
 */
PumpMasterflex::PumpMasterflex(MasterflexDB25Interface_t bd25)
{
    _pins.direction_pin = bd25.direction_pin;
    _pins.prime_pin = bd25.prime_pin;
    _pins.start_stop_pin = bd25.start_stop_pin;
    _pins.voltage_in_pin = bd25.voltage_in_pin;
    _pins.voltage_out_pin = bd25.voltage_out_pin;
}

/**
 * @brief Perform initializations on Arduino for the pump.
 * @brief Set initial state parameters read from the pump
 * @return uint16_t 
 */
uint16_t PumpMasterflex::Connect()
{
    pinMode(_pins.voltage_in_pin, OUTPUT);
    pinMode(_pins.start_stop_pin, OUTPUT);
    pinMode(_pins.direction_pin, OUTPUT);
    pinMode(_pins.prime_pin, OUTPUT);
    _state_op = this->GetOpState();
    _state_dir = this->GetDirection();
    _speed_control.speed_ml_min = 0.0;
    this->SetSpeed(0.0);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Start the pump by pulling the START/STOP pin to LOW
/// @note The pump wil run at the set speed in the system
uint16_t PumpMasterflex::Start()
{
    digitalWrite(_pins.start_stop_pin, PUMP_START);
    _state_op = PUMP_START;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Stop the pump by pulling the START/STOP pin to HIGH
uint16_t PumpMasterflex::Stop()
{
    digitalWrite(_pins.start_stop_pin, PUMP_STOP);
    _state_op = PUMP_STOP;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Retrieve the current state of START/STOP pin of the pump
/// @return @c 0/LOW  the pump is running
///         @c 1/HIGH the pump is idle
uint16_t PumpMasterflex::GetOpState()
{
    _state_op = digitalRead(_pins.start_stop_pin);
    return _state_op;
}

/// @brief Set the direction (clockwise/counterclockwise) by the CW/CCW pin
/// @param direction @c 1/HIGH CW  @c 0/LOW CCW
uint16_t PumpMasterflex::SetDirection(uint32_t direction)
{
    if (direction != DIR_CW && direction != DIR_CCW)
    {
        return RESP_GENERAL_ERROR_INVALID_PARAMS;
    }
    _state_dir = (uint8_t)direction;
    digitalWrite(_pins.direction_pin, _state_dir);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Toggle the direction (clockwise/counterclockwise) by the CW/CCW pin
uint16_t PumpMasterflex::ChangeDirection()
{
    _state_dir = _state_dir ^ HIGH;
    digitalWrite(_pins.direction_pin, _state_dir);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Retrieve the current state of CW/CCW pin of the pump
/// @return @c 0/LOW  the pump is set counterclockwise
///         @c 1/HIGH the pump is set clockwise
uint16_t PumpMasterflex::GetDirection()
{
    _state_dir = digitalRead(_pins.direction_pin);
    return _state_dir;
}

/// @brief Retrieve the setup tube size
/// @return tube_size: 13, 14, 16, ...
uint16_t PumpMasterflex::GetTubeSize(void)
{
    return _tube.size;
}

/// @brief Update @c speed_control (max_speed and min_speed) by the size of tube
/// @param size tube size (13, 14, 16, etc)
uint16_t PumpMasterflex::SetTubeSize(uint32_t size)
{
    _tube.size = (uint8_t)size;
    switch (size){
        case TUBE_SIZE_13:
        {
            _tube.max_speed = TUBE_MAX_SPEED_13;
            _tube.min_speed = TUBE_MIN_SPEED_13;
            break;
        }
        case TUBE_SIZE_14:
        {
            _tube.max_speed = TUBE_MAX_SPEED_14;
            _tube.min_speed = TUBE_MIN_SPEED_14;
            break;
        }
        case TUBE_SIZE_16:
        default:
        {
            _tube.max_speed = TUBE_MAX_SPEED_16;
            _tube.min_speed = TUBE_MIN_SPEED_16;
            break;
        }
    }
    _speed_control.max_speed = _tube.max_speed;
    _speed_control.min_speed = _tube.min_speed;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the pump max speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed maximum speed by @c ul/min
/// @note converted to @c ml/min
uint16_t PumpMasterflex::SetMaxSpeed(uint32_t speed)
{
    _speed_control.max_speed = ((float)speed / 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the pump min speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed minimum speed by @c ul/min
/// @note converted to @c ml/min
uint16_t PumpMasterflex::SetMinSpeed(uint32_t speed)
{
    _speed_control.min_speed =  ((float)speed / 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param speed 
 * @return uint16_t 
 */
uint16_t PumpMasterflex::GetMaxSpeed(uint32_t * speed)
{
    (*speed) =  (uint32_t)(_speed_control.max_speed * 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param speed 
 * @return uint16_t 
 */
uint16_t PumpMasterflex::GetMinSpeed(uint32_t * speed)
{
    (*speed) =  (uint32_t)(_speed_control.min_speed * 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the pump speed by ml/min value
/// @attention the pump speed ml/min values are both updated
/// @note The continuous mode is not accurate due to fluctuating voltage signals
/// @param speed_ul_min from min_speed to max_speed (ul/min)
uint16_t PumpMasterflex::SetSpeed(uint32_t speed_ul_min)
{
    float min_speed = _speed_control.min_speed;
    float max_speed = _speed_control.max_speed;
    _speed_control.speed_ml_min = ((float)speed_ul_min / 1000);
    if (_speed_control.speed_ml_min < min_speed)
    {
        _speed_control.speed_ml_min = min_speed;
    }
    float speed_percent = (_speed_control.speed_ml_min - min_speed) / (max_speed - min_speed);
    int pwm_val = (int)(speed_percent * ARDUINO_PWM);
    analogWrite(_pins.voltage_in_pin, pwm_val);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Read the INPUT_VOLTAGE analog pin and calculate the current pump speed (by ml/min)
/// @return pump speed by ul/min
uint16_t PumpMasterflex::GetSpeed(uint32_t * speed)
{
    // analog read range 0-1023. operating voltage is 5V
    float voltage = ((float)analogRead(_pins.voltage_out_pin)*5) / 1023;
    // Serial.println(voltage);
    if (voltage < _min_voltage)
    {
        voltage = _min_voltage;
    }
    float percent   = (voltage - _min_voltage)/(_max_voltage-_min_voltage);
    // Serial.println(percent);
    (*speed) = (uint32_t)(1000 * (percent * (_speed_control.max_speed-_speed_control.min_speed) + _speed_control.min_speed));
    // Serial.println(speed);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Get the speed in software setting
/// @return speed in ul/min
uint16_t PumpMasterflex::GetSpeedSetting(uint32_t * speed)
{
    (*speed) =  (uint32_t)(_speed_control.speed_ml_min * 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the voltage threshold for the pump @c VOLTAGE_OUTPUT 
/// @param voltage_max highest voltage for the OUTPUT_PIN, in @c mV
/// @warning Must be updated manually following the setting on the pump
uint16_t PumpMasterflex::SetMaxVoltageLevel(uint32_t voltage_max)
{
    _max_voltage = ((float)voltage_max / 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}
/// @brief Set the voltage threshold for the pump @c VOLTAGE_OUTPUT
/// @param voltage_min lowest voltage for the OUTPUT_PIN, in @c mV
/// @warning Must be updated manually following the setting on the pump
uint16_t PumpMasterflex::SetMinVoltageLevel(uint32_t voltage_min)
{
    _min_voltage = ((float)voltage_min / 1000);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Dispense a predetermined amount of liquid
/// @param amount_ul amount of dispensed liquid in uL
/// @return true/1 if success
uint16_t PumpMasterflex::Dispense(uint32_t amount_ul)
{
    uint64_t time;
    if (_pipe_state == PIPE_EMPTY)
    {
        time = (uint64_t)((60 * (amount_ul + _pipe_vol + _tuning_vol)) / (_speed_control.speed_ml_min));
        // _pipe_state = PIPE_PRIMED;
    }
    else
    {
        time = (uint64_t)((60 * (amount_ul + _tuning_vol)) / (_speed_control.speed_ml_min));
    }
    int res = this->Start();
    delay(time);
    res = this->Stop();
    return res;
}

/// @brief Set the amount of liquid used to fill pipes (only use the first time for each liquid)
/// @param vol_ul volume in uL
/// @return 1/true
uint16_t PumpMasterflex::PipeSetVol(uint32_t vol_ul)
{
    _pipe_vol = vol_ul;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the amount of liquid used to fill pipes (only use the first time for each liquid)
/// @param vol_ul volume in uL
/// @return 1/true
uint16_t PumpMasterflex::PipeSetState(uint32_t state)
{
    _pipe_state = (uint8_t)state;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/// @brief Set the tuning volume for the pump (after calibration)
/// @param amount_ul amount in uL
/// @return true
uint16_t PumpMasterflex::SetTuningVol(uint32_t amount_ul)
{
    _tuning_vol = amount_ul;
    return RESP_GENERAL_FEEDBACK_VOID;
}