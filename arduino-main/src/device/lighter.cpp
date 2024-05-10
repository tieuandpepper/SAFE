#include "lighter.h"

/**
 * @brief Construct a new Lighter:: Lighter object
 * 
 * @param enable_pin 
 */
Lighter::Lighter(uint8_t enable_pin)
{
    _enable_pin = enable_pin;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t Lighter::Connect()
{
    pinMode(_enable_pin,OUTPUT);
    digitalWrite(_enable_pin,LOW);
    delay(200);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param time_ms 
 * @return uint16_t 
 */
uint16_t Lighter::Ignite(uint32_t time_ms = DEFAULT_IGNITION_TIME_MS)
{
    digitalWrite(_enable_pin,HIGH);
    delay(time_ms);
    digitalWrite(_enable_pin,LOW);
    return RESP_GENERAL_FEEDBACK_VOID;
}