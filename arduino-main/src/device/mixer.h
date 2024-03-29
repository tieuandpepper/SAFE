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

#define STATE_OFF     0
#define STATE_RUNNING 1
#define STATE_IDLE    2

class Mixer {
  private:
    uint8_t _state = STATE_OFF;
    uint8_t _pin;
  public:
    void Start();
    void Stop();
    void Run(int32_t time_ms);
};

#endif