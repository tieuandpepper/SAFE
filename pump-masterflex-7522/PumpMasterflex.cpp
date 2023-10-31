/**
 * 
*/

#include "Arduino.h"
#include "PumpMasterflex.h"

PumpMasterflex::PumpMasterflex(MasterflexDB25Interface_t bd25)
{
    _pins.direction_pin = bd25.direction_pin;
    _pins.prime_pin = bd25.prime_pin;
    _pins.start_stop_pin = bd25.start_stop_pin;
    _pins.voltage_in_pin = bd25.voltage_in_pin;
    _pins.voltage_out_pin = bd25.voltage_out_pin;
}

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

void PumpMasterflex::Start()
{
    this->state_op = PUMP_ON;
    digitalWrite(_pins.start_stop_pin, PUMP_ON);
}

void PumpMasterflex::Stop()
{
    this->state_op = PUMP_OFF;
    digitalWrite(_pins.start_stop_pin, PUMP_OFF);
}

void PumpMasterflex::PrimeStart()
{
    this->state_prime = PRIME_ON;
    digitalWrite(_pins.prime_pin, PRIME_ON);
}

void PumpMasterflex::PrimeStop()
{
    this->state_prime = PRIME_OFF;
    digitalWrite(_pins.prime_pin, PRIME_OFF);
}
void PumpMasterflex::Prime(uint64_t duration_ms)
{
    this->PrimeStart();
    delay(duration_ms);
    this->PrimeStop();
}

void PumpMasterflex::SetDirection(uint8_t direction)
{
    this->state_dir = direction;
    digitalWrite(_pins.direction_pin, direction);
}

void PumpMasterflex::ChangeDirection()
{
    this->state_dir = this->state_dir ^ HIGH;
    digitalWrite(_pins.direction_pin, this->state_dir);
}

void PumpMasterflex::SetTubeSize(uint8_t size)
{
    this->tube_size = tube_size(size);
}

void PumpMasterflex::SetVoltageLevel(uint8_t voltage_max, uint8_t voltage_min = 0);
void PumpMasterflex::SetSpeedPercent(double percent);
void PumpMasterflex::SetSpeed(double speed_ml_min);
void PumpMasterflex::SetMaxSpeed(double speed);
void PumpMasterflex::SetMinSpeed(double speed);
uint8_t PumpMasterflex::GetOpState();
uint8_t PumpMasterflex::GetDirection();
uint8_t PumpMasterflex::GetPrimeState();
uint8_t PumpMasterflex::GetTubeSize();
double PumpMasterflex::GetSpeedPercent();
double PumpMasterflex::GetSpeed();
