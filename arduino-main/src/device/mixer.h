/**
 * @file mixer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef Mixer_h
#define Mixer_h

#include "Arduino.h"
#include "response_error_code.h"

#define STATE_OFF     1
#define STATE_RUNNING 2
#define STATE_IDLE    3

class Mixer {
  private:
    uint8_t _state = STATE_OFF;
    uint8_t _pin;
  public:
    Mixer(uint8_t pin);
    uint16_t Connect();
    uint16_t Start();
    uint16_t Stop();
    uint16_t Run(uint32_t time_ms);
};

#endif