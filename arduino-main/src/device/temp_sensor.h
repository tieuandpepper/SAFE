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

#define BUFFER_SIZE  100

#define ERROR_OKAY           0
#define ERROR_SENSOR         1
#define ERROR_NO_BUFFER_DATA 2

#define TIME_EXPIRED         1
#define TIME_AVAILABLE       0

class TempSensorMAX31855{
    private:
        DFRobot_MAX31855 _sensor_obj;
        float _data_buffer[BUFFER_SIZE];
        int32_t _current_idx;
        uint32_t _data_count;
        unsigned long _start_time;
        unsigned long _end_time;
        
    public:
        TempSensorMAX31855();
        uint8_t ReadSensor(float * data);
        uint8_t GetLastData(float * data);
        uint8_t ReadSensorToBuffer(float * data);
        uint8_t InitializeReadContinuous(unsigned long duration_ms);
        uint8_t CheckEndTime();
}


#endif