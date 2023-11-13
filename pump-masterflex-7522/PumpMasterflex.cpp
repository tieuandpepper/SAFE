/**
 * 
*/

#include "Arduino.h"
#include "PumpMasterflex.h"

#define DELAY_MS (5)

/// @brief PumpMasterflex constructor. Must be initialized first.
/// @param bd25 contains a struct of input/output pins on Arduino
PumpMasterflex::PumpMasterflex(MasterflexDB25Interface_t bd25)
{
    _pins.direction_pin = bd25.direction_pin;
    _pins.prime_pin = bd25.prime_pin;
    _pins.start_stop_pin = bd25.start_stop_pin;
    _pins.voltage_in_pin = bd25.voltage_in_pin;
    _pins.voltage_out_pin = bd25.voltage_out_pin;
}

/// @brief Perform initializations on Arduino for the pump.
/// @brief Set initial state parameters read from the pump
bool PumpMasterflex::Connect()
{
    pinMode(_pins.voltage_out_pin, OUTPUT);
    pinMode(_pins.start_stop_pin, OUTPUT);
    pinMode(_pins.direction_pin, OUTPUT);
    pinMode(_pins.prime_pin, OUTPUT);
    _state_op = this->GetOpState();
    _state_dir = this->GetDirection();
    _state_prime = this->GetPrimeState();
    _speed_control.speed_percent = this->GetSpeedPercent();
    _speed_control.speed_ml_min = this->GetSpeed();
    return true;
}

/// @brief Start the pump by pulling the START/STOP pin to LOW
/// @note The pump wil run at the set speed in the system
bool PumpMasterflex::Start()
{
    digitalWrite(_pins.start_stop_pin, PUMP_START);
    delay(DELAY_MS);
    if (digitalRead(_pins.start_stop_pin) != PUMP_START)
    {
        return false;
    }
    _state_op = PUMP_START;
    return true;
}

/// @brief Stop the pump by pulling the START/STOP pin to HIGH
bool PumpMasterflex::Stop()
{
    digitalWrite(_pins.start_stop_pin, _state_op);
    delay(DELAY_MS);
    if (digitalRead(_pins.start_stop_pin) != PUMP_STOP)
    {
        return false;
    }
    _state_op = PUMP_STOP;
    return true;
}

/// @brief Retrieve the current state of START/STOP pin of the pump
/// @return @c 0/LOW  the pump is running
///         @c 1/HIGH the pump is idle
uint8_t PumpMasterflex::GetOpState()
{
    _state_op = digitalRead(_pins.start_stop_pin);
    return _state_op;
}

/// @brief Set the direction (clockwise/counterclockwise) by the CW/CCW pin
/// @param direction @c 1/HIGH CW  @c 0/LOW CCW
bool PumpMasterflex::SetDirection(uint32_t direction)
{
    digitalWrite(_pins.direction_pin, direction);
    delay(DELAY_MS);
    if (digitalRead(_pins.start_stop_pin) != direction)
    {
        return false;
    }
    _state_dir = direction;
    return true;
}

/// @brief Toggle the direction (clockwise/counterclockwise) by the CW/CCW pin
bool PumpMasterflex::ChangeDirection()
{
    _state_dir = _state_dir ^ HIGH;
    digitalWrite(_pins.direction_pin, _state_dir);
    delay(DELAY_MS);
    if (digitalRead(_pins.direction_pin) != _state_dir)
    {
        _state_dir = _state_dir ^ HIGH;
        return false;
    }
    return true;
}

/// @brief Retrieve the current state of CW/CCW pin of the pump
/// @return @c 0/LOW  the pump is set counterclockwise
///         @c 1/HIGH the pump is set clockwise
uint8_t PumpMasterflex::GetDirection()
{
    _state_dir = digitalRead(_pins.direction_pin);
    return _state_dir;
}

/// @brief Start priming by pulling the Remote Prime pin to HIGH
bool PumpMasterflex::PrimeStart()
{
    digitalWrite(_pins.prime_pin, PRIME_ON);
    delay(DELAY_MS);
    if (digitalRead(_pins.direction_pin) != PRIME_ON)
    {
        return false;
    }
    _state_prime = PRIME_ON;
    return true;
}

/// @brief Stop priming by pulling the Remote Prime pin to LOW
bool PumpMasterflex::PrimeStop()
{
    delay(DELAY_MS);
    if (digitalRead(_pins.direction_pin) != PRIME_OFF)
    {
        return false;
    }
    _state_prime = PRIME_OFF;
    return true;
}

/// @brief Prime the pump by a predetermined period.
/// @warning Pump is unavailable during this period.
/// @param duration_ms priming duration by @c miliseconds (ms)
bool PumpMasterflex::Prime(uint32_t duration_ms)
{
    bool res = this->PrimeStart();
    delay(duration_ms);
    res |= this->PrimeStop();
    return res;
}

/// @brief Retrieve the current state of REMOTE PRIME pin of the pump
/// @return @c 0/LOW  the pump is priming
///         @c 1/HIGH the pump is not priming
uint8_t PumpMasterflex::GetPrimeState(void)
{
    _state_prime = digitalRead(_pins.prime_pin);
    return _state_prime;
}

/// @brief Retrieve the setup tube size
/// @return tube_size: 13, 14, 16, ...
uint8_t PumpMasterflex::GetTubeSize(void)
{
    return _tube.size;
}

/// @brief Update @c speed_control (max_speed and min_speed) by the size of tube
/// @param size tube size (13, 14, 16, etc)
bool PumpMasterflex::SetTubeSize(uint32_t size)
{
    _tube.size = size;
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
    return true;
}

/// @brief Set the pump max speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed maximum speed by @c ul/min
/// @note return in @c ml/min
bool PumpMasterflex::SetMaxSpeed(uint32_t speed)
{
    _speed_control.max_speed = (double)speed / 1000;
    return true;
}

/// @brief Set the pump min speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed minimum speed by @c ul/min
/// @note return in @c ml/min
bool PumpMasterflex::SetMinSpeed(uint32_t speed)
{
    _speed_control.min_speed = (double)speed / 1000;
    return true;
}

/// @brief Set the pump speed by percentage
/// @attention the pump speed percentage and ml/min values are both updated
/// @param percent double from 0-100 (%)
bool PumpMasterflex::SetSpeedPercent(uint32_t percent)
{
    double min_speed = _speed_control.min_speed;
    double max_speed = _speed_control.max_speed;
    _speed_control.speed_percent = (double)percent / 100;
    _speed_control.speed_ml_min = min_speed + (_speed_control.speed_percent * (max_speed - min_speed));
    int pwm_val = (int)(_speed_control.speed_percent * ARDUINO_PWM);
    analogWrite(_pins.voltage_in_pin, pwm_val);
    return true;
}

/// @brief Set the pump speed by ml/min value
/// @attention the pump speed percentage and ml/min values are both updated
/// @note The continuous mode is not accurate due to fluctuating voltage signals
/// @param speed_ml_min double from min_speed to max_speed (ml/min)
bool PumpMasterflex::SetSpeed(uint32_t speed_ml_min)
{
    double min_speed = _speed_control.min_speed;
    double max_speed = _speed_control.max_speed;
    _speed_control.speed_ml_min = (double)speed_ml_min / 1000;
    _speed_control.speed_percent = (_speed_control.speed_ml_min - min_speed) / (max_speed - min_speed);
    int pwm_val = (int)(_speed_control.speed_percent * ARDUINO_PWM);
    analogWrite(_pins.voltage_in_pin, pwm_val);
    return true;
}

/// @brief Read the INPUT_VOLTAGE analog pin and calculate the current pump speed (by % and by ml/min)
/// @brief Update @c speed_control (speed_percent and speed_ml_min)
/// @return pump speed by percent (0-100)
uint32_t PumpMasterflex::GetSpeedPercent()
{    
    return (uint32_t)(_speed_control.speed_percent*100);
}

/// @brief Read the INPUT_VOLTAGE analog pin and calculate the current pump speed (by % and by ml/min)
/// @brief Update @c speed_control (speed_percent and speed_ml_min)
/// @return pump speed by ul/min
uint32_t PumpMasterflex::GetSpeed(void)
{
    return (uint32_t)(_speed_control.speed_ml_min*1000);
}

/// @brief Set the voltage threshold for the pump @c VOLTAGE_OUTPUT 
/// @param voltage_max highest voltage for the OUTPUT_PIN, in @c mV
/// @warning Must be updated manually following the setting on the pump
bool PumpMasterflex::SetMaxVoltageLevel(uint32_t voltage_max)
{
    _max_voltage = voltage_max;
    return true;
}
/// @brief Set the voltage threshold for the pump @c VOLTAGE_OUTPUT
/// @param voltage_min lowest voltage for the OUTPUT_PIN, in @c mV
/// @warning Must be updated manually following the setting on the pump
bool PumpMasterflex::SetMinVoltageLevel(uint32_t voltage_min)
{
    _min_voltage = voltage_min;
    return true;
}

bool PumpMasterflex::Dispense(uint32_t amount_ul)
{
    uint64_t time = (60000 * amount_ul) / (speed);
    bool res = this->Start();
    delay(time);
    res |= this->Stop();
    return res;
}
