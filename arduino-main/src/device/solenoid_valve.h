/**
 * @file solenoid_valve.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SOLENOID_VALVE_H
#define SOLENOID_VALVE_H

#include "Arduino.h"
#include "response_error_code.h"

#define STATE_OFF     1
#define STATE_RUNNING 2
#define STATE_IDLE    3

class SolenoidValve {
  private:
    uint8_t _state = STATE_OFF;
    uint8_t _pin;
  public:
    SolenoidValve(uint8_t pin);
    uint16_t Connect();
    uint16_t Start();
    uint16_t Stop();
    uint16_t Run(uint32_t time_ms);
};

#endif