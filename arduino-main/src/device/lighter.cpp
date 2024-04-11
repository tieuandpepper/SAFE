#include "lighter.h"

/**
 * @brief Construct a new Lighter:: Lighter object
 * 
 * @param init_pin 
 * @param enable_pin 
 */
Lighter::Lighter(uint8_t init_pin, uint8_t enable_pin)
{
    _init_pin = init_pin;
    _enable_pin = enable_pin;
}

/**
 * @brief 
 * 
 * @return int 
 */
int Lighter::Connect()
{
    pinMode(_enable_pin,OUTPUT);
    digitalWrite(_enable_pin,LOW);
    pinMode(_init_pin,OUTPUT);
    digitalWrite(_init_pin,HIGH);
    delay(200);
    digitalWrite(_init_pin,LOW);
    return RESP_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param time_ms 
 * @return int 
 */
int Lighter::Ignite(int time_ms = DEFAULT_IGNITION_TIME_MS)
{
    digitalWrite(_enable_pin,HIGH);
    delay(time_ms);
    digitalWrite(_enable_pin,LOW);
    return RESP_FEEDBACK_VOID;
}