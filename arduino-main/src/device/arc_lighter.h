/**
 * @file arc_lighter.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ARCLIGHTER_H
#define ARCLIGHTER_H

#include "Arduino.h"
#include "response_error_code.h"

#define DEFAULT_IGNITION_TIME_MS 1000
#define STATUS_OFF 0
#define STATUS_ON  1

class ArcLighter {
    private:
        uint8_t _enable_pin;
        uint8_t _charger_pin;
        uint8_t _status;
        uint32_t _ignition_duration;
        uint32_t _ignition_time;
    public:
        ArcLighter(uint8_t enable, uint8_t charger);
        void SetEnablePin(uint8_t enable) {_enable_pin = enable;};
        void SetChargerPin(uint8_t charger) {_charger_pin = charger;};
        uint16_t Connect();
        uint16_t Ignite(uint32_t time_ms = DEFAULT_IGNITION_TIME_MS);
        uint16_t Charging();
        uint16_t Uncharging();
        uint16_t CheckStop();
        uint8_t  GetStatus() {return _status;}; 
};


#endif