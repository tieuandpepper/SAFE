/**
 * @file solenoid_valve.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "solenoid_valve.h"

/**
 * @brief Construct a new SolenoidValve:: SolenoidValve object
 * 
 * @param pin 
 */
SolenoidValve::SolenoidValve(uint8_t pin)
{
  _pin = pin;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return uint16_t 
 */
uint16_t SolenoidValve::Connect()
{
  pinMode(_pin,OUTPUT);
  digitalWrite(_pin, LOW);
  _state = STATE_IDLE;
  return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return uint16_t 
 */
uint16_t SolenoidValve::Start()
{
  digitalWrite(_pin, HIGH);
  _state = STATE_RUNNING;
  return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return uint16_t 
 */
uint16_t SolenoidValve::Stop()
{
  digitalWrite(_pin, LOW);
  _state = STATE_IDLE;
  return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param pin 
 * @return uint16_t 
 */
uint16_t SolenoidValve::Run(uint32_t time_ms)
{
  this->Start();
  delay(time_ms);
  this->Stop();
  return RESP_GENERAL_FEEDBACK_VOID;
}