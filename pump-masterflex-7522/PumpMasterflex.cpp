/**
 * 
*/

#include "Arduino.h"
#include "PumpMasterflex.h"

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
void PumpMasterflex::Connect()
{
    pinMode(_pins.voltage_out_pin, OUTPUT);
    pinMode(_pins.start_stop_pin, OUTPUT);
    pinMode(_pins.direction_pin, OUTPUT);
    pinMode(_pins.prime_pin, OUTPUT);
    this->state_op = this->GetOpState();
    this->state_dir = this->GetDirection();
    this->state_prime = this->GetPrimeState();
    this->speed_control.speed_percent = this->GetSpeedPercent();
    this->speed_control.speed_ml_min = this->GetSpeed();
}

/// @brief Start the pump by pulling the START/STOP pin to LOW
void PumpMasterflex::Start()
{
    this->state_op = PUMP_ON;
    digitalWrite(_pins.start_stop_pin, PUMP_ON);
}

/// @brief Stop the pump by pulling the START/STOP pin to HIGH
void PumpMasterflex::Stop()
{
    this->state_op = PUMP_OFF;
    digitalWrite(_pins.start_stop_pin, PUMP_OFF);
}

/// @brief Retrieve the current state of START/STOP pin of the pump
/// @return @c 0/LOW  the pump is running
///         @c 1/HIGH the pump is idle
uint8_t PumpMasterflex::GetOpState()
{
    this->state_op = digitalRead(_pins.start_stop_pin);
    return this->state_op;
}

/// @brief Retrieve the current state of CW/CCW pin of the pump
/// @return @c 0/LOW  the pump is set counterclockwise
///         @c 1/HIGH the pump is set clockwise
uint8_t PumpMasterflex::GetDirection()
{
    this->state_dir = digitalRead(_pins.direction_pin);
    return this->state_dir;
}

/// @brief Retrieve the current state of REMOTE PRIME pin of the pump
/// @return @c 0/LOW  the pump is priming
///         @c 1/HIGH the pump is not priming
uint8_t PumpMasterflex::GetPrimeState()
{
    this->state_prime = digitalRead(_pins.prime_pin);
    return this->state_prime;
}

/// @brief Retrieve the setup tube size
/// @return tube_size: 13, 14, 16, ...
uint8_t PumpMasterflex::GetTubeSize()
{
    return this->tube.size;
}

/// @brief Read the INPUT_VOLTAGE analog pin and calculate the current pump speed (by % and by ml/min)
/// @brief Update @c speed_control (speed_percent and speed_ml_min)
/// @return pump speed by percent
double PumpMasterflex::GetSpeedPercent()
{    
    double max_speed = this->speed_control.max_speed;
    double min_speed = this->speed_control.min_speed;
    int analog_val = analogRead(_pins.voltage_out_pin);
    double voltage = (double)analog_val * ARDUINO_VCC / ARDUINO_ANALOG;
    this->speed_control.speed_percent = (voltage - this->min_voltage) * 100 / (this->max_voltage - this->min_voltage);
    this->speed_control.speed_ml_min = this->speed_control.speed_percent * (max_speed - min_speed) / 100 + min_speed;
    return this->speed_control.speed_percent;
}

/// @brief Read the INPUT_VOLTAGE analog pin and calculate the current pump speed (by % and by ml/min)
/// @brief Update @c speed_control (speed_percent and speed_ml_min)
/// @return pump speed by ml/min
double PumpMasterflex::GetSpeed()
{
    double max_speed = this->speed_control.max_speed;
    double min_speed = this->speed_control.min_speed;
    int analog_val = analogRead(_pins.voltage_out_pin);
    double voltage = (double)analog_val * ARDUINO_VCC / ARDUINO_ANALOG;
    this->speed_control.speed_percent = (voltage - this->min_voltage) * 100 / (this->max_voltage - this->min_voltage);
    this->speed_control.speed_ml_min = this->speed_control.speed_percent * (max_speed - min_speed) / 100 + min_speed;
    return this->speed_control.speed_ml_min;
}

/// @brief Start priming by pulling the Remote Prime pin to HIGH
void PumpMasterflex::PrimeStart()
{
    this->state_prime = PRIME_ON;
    digitalWrite(_pins.prime_pin, PRIME_ON);
}

/// @brief Stop priming by pulling the Remote Prime pin to LOW
void PumpMasterflex::PrimeStop()
{
    this->state_prime = PRIME_OFF;
    digitalWrite(_pins.prime_pin, PRIME_OFF);
}

/// @brief Prime the pump by a predetermined period.
/// @warning Pump is unavailable during this period.
/// @param duration_ms priming duration by @c miliseconds (ms)
void PumpMasterflex::Prime(uint64_t duration_ms)
{
    this->PrimeStart();
    delay(duration_ms);
    this->PrimeStop();
}

/// @brief Set the direction (clockwise/counterclockwise) by the CW/CCW pin
/// @param direction @c 1/HIGH CW  @c 0/LOW CCW
void PumpMasterflex::SetDirection(uint8_t direction)
{
    this->state_dir = direction;
    digitalWrite(_pins.direction_pin, direction);
}

/// @brief Toggle the direction (clockwise/counterclockwise) by the CW/CCW pin
void PumpMasterflex::ChangeDirection()
{
    this->state_dir = this->state_dir ^ HIGH;
    digitalWrite(_pins.direction_pin, this->state_dir);
}

/// @brief Update @c speed_control (max_speed and min_speed) by the size of tube
/// @param size tube size (13, 14, 16, etc)
void PumpMasterflex::SetTubeSize(uint8_t size)
{
    this->tube.size = size;
    switch (size){
        case TUBE_SIZE_13:
        {
            this->tube.max_speed = TUBE_MAX_SPEED_13;
            this->tube.min_speed = TUBE_MIN_SPEED_13;
            break;
        }
        case TUBE_SIZE_14:
        {
            this->tube.max_speed = TUBE_MAX_SPEED_14;
            this->tube.min_speed = TUBE_MIN_SPEED_14;
            break;
        }
        case TUBE_SIZE_16:
        default:
        {
            this->tube.max_speed = TUBE_MAX_SPEED_16;
            this->tube.min_speed = TUBE_MIN_SPEED_16;
            break;
        }
    }
    this->speed_control.max_speed = this->tube.max_speed;
    this->speed_control.min_speed = this->tube.min_speed;
}

/// @brief Set the pump max speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed maximum speed by @c ml/min
void PumpMasterflex::SetMaxSpeed(double speed)
{
    this->speed_control.max_speed = speed;
}

/// @brief Set the pump min speed manually
/// @attention This is used to overwrite the default tube size setting
/// @param speed minimum speed by @c ml/min
void PumpMasterflex::SetMinSpeed(double speed)
{
    this->speed_control.min_speed = speed;
}

/// @brief Set the voltage threshold for the pump @c VOLTAGE_OUTPUT 
/// @param voltage_max highest voltage for the OUTPUT_PIN
/// @param voltage_min lowest voltage for the OUTPUT_PIN
/// @warning Must be updated manually following the setting on the pump
void PumpMasterflex::SetVoltageLevel(uint8_t voltage_max, uint8_t voltage_min = 0)
{
    this->max_voltage = voltage_max;
    this->min_voltage = voltage_min;
}

/// @brief Set the pump speed by percentage
/// @attention the pump speed percentage and ml/min values are both updated
/// @param percent double from 0-100 (%)
void PumpMasterflex::SetSpeedPercent(double percent)
{
    double min_speed = this->speed_control.min_speed;
    double max_speed = this->speed_control.max_speed;
    this->speed_control.speed_percent = percent;
    this->speed_control.speed_ml_min = percent * (max_speed - min_speed) / 100 + min_speed;
    int pwm_val = (int)(percent * ARDUINO_PWM / 100);
    analogWrite(_pins.voltage_in_pin, pwm_val);
}

/// @brief Set the pump speed by ml/min value
/// @attention the pump speed percentage and ml/min values are both updated
/// @param speed_ml_min double from min_speed to max_speed (ml/min)
void PumpMasterflex::SetSpeed(double speed_ml_min)
{
    double min_speed = this->speed_control.min_speed;
    double max_speed = this->speed_control.max_speed;
    this->speed_control.speed_ml_min = speed_ml_min;
    this->speed_control.speed_percent = (speed_ml_min - min_speed) * 100 / (max_speed - min_speed);
    int pwm_val = (int)(speed_ml_min * ARDUINO_PWM / (max_speed - min_speed));
    analogWrite(_pins.voltage_in_pin, pwm_val);
}
