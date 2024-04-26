/**
 * @file temp_sensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <DFRobot_MAX31855.h>
#include <Arduino.h>
#include "response_error_code.h"

#define BUFFER_SIZE  100

#define TIME_EXPIRED         0xEEEE
#define TIME_AVAILABLE       0xAAAA

#define FLAG_READ_ONCE       0x1111
#define FLAG_READ_DURATION   0xCCCC
#define FLAG_READ_PERIOD     0xDDDD

class TempSensorMAX31855: public DFRobot_MAX31855 {
    private:
        uint32_t _start_time;
        uint32_t _period_time;
        uint32_t _end_time;
        uint16_t _read_flag;
        uint16_t CheckEndTime();
        
    public:
        TempSensorMAX31855();
        uint16_t Connect();
        uint16_t ReadSensor(float * data);
        uint16_t InitializeReadPeriod(uint32_t period_ms);
        uint16_t InitializeReadDuration(uint32_t duration_ms);
        uint16_t ReadSensorbyDuration(float *data);
        uint16_t ReadSensorbyPeriod(float *data);
        uint16_t StopRead();
        bool DurationFlagOn();
        bool PeriodFlagOn();
};


#endif