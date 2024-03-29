/**
 * @file mixer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mixer.h"

/**
 * @brief 
 * 
 */
void Mixer::Start()
{
  digitalWrite(_pin, HIGH);
  _state = STATE_RUNNING;
}

/**
 * @brief 
 * 
 */
void Mixer::Stop()
{
  digitalWrite(_pin, LOW);
  _state = STATE_IDLE;
}

/**
 * @brief 
 * 
 * @param time_ms 
 */
void Mixer::Run(int32_t time_ms)
{
  this->Start();
  _state = STATE_RUNNING;
  delay(time_ms);
  this->Stop();
}