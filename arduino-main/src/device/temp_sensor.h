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
#include "response_code.h"

#define BUFFER_SIZE  100

#define TIME_EXPIRED         0xEEEE
#define TIME_AVAILABLE       0xAAAA

#define FLAG_READ_ONCE       0x1111
#define FLAG_READ_CONT       0xCCCC

class TempSensorMAX31855{
    private:
        DFRobot_MAX31855 _sensor_obj;
        unsigned long _start_time;
        unsigned long _end_time;
        int _read_flag;
        int CheckEndTime();
        
    public:
        TempSensorMAX31855();
        int ReadSensor(float * data);
        int InitializeReadContinuous(unsigned long duration_ms);
        int ReadSensorContinous(float *data);
        bool ContinuousFlagOn();
};


#endif