/**
 * @file lighter.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LIGHTER_H
#define LIGHTER_H

#include "Arduino.h"
#include "response_error_code.h"

#define DEFAULT_IGNITION_TIME_MS 1000

class Lighter {
    private:
        uint8_t _enable_pin;
    public:
        Lighter(uint8_t enable_pin);
        uint16_t Connect();
        uint16_t Ignite(uint32_t time_ms = DEFAULT_IGNITION_TIME_MS);
};


#endif