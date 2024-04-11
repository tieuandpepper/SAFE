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
 * @brief Construct a new Mixer:: Mixer object
 * 
 * @param pin 
 */
Mixer::Mixer(uint8_t pin)
{
  _pin = pin;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return int 
 */
int Mixer::Connect()
{
  pinMode(_pin,OUTPUT);
  digitalWrite(_pin, LOW);
  _state = STATE_IDLE;
  return RESP_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return int 
 */
int Mixer::Start()
{
  digitalWrite(_pin, HIGH);
  _state = STATE_RUNNING;
  return RESP_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return int 
 */
int Mixer::Stop()
{
  digitalWrite(_pin, LOW);
  _state = STATE_IDLE;
  return RESP_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return int 
 */
int Mixer::Run(int32_t time_ms)
{
  this->Start();
  delay(time_ms);
  this->Stop();
  return RESP_FEEDBACK_VOID;
}