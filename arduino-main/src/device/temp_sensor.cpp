/**
 * @file temp_sensor.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "temp_sensor.h"

/**
 * @brief Construct a new Temp Sensor
 * 
 */
TempSensorMAX31855::TempSensorMAX31855()
{
    _sensor_obj.begin();
    _data_count = 0;
    _current_idx = 0;
}

/**
 * @brief read sensor, return the reading and update the data buffer
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t TempSensorMAX31855::ReadSensor(float * data)
{
    (*data) = _sensor_obj.readCelsius();
    if ((*data) == NAN)
    {
        return ERROR_SENSOR;
    }
    return ERROR_OKAY;
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t TempSensorMAX31855::GetLastData(float * data)
{
    if (_data_count == 0) {return ERROR_NO_BUFFER_DATA;}
    (*data) = _data_buffer[_current_idx];
    return ERROR_OKAY;
}

/**
 * @brief 
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t TempSensorMAX31855::ReadSensorToBuffer(float * data)
{
    (*data) = _sensor_obj.readCelsius();
    if ((*data) == NAN)
    {
        return ERROR_SENSOR;
    }
    _current_idx = (_current_idx+1) % BUFFER_SIZE;
    _data_buffer[_current_idx] = (*data);
    _data_count++;
    return ERROR_OKAY;
}

/**
 * @brief initialize continuous read mode until time run out
 * 
 * @param duration
 * @return uint8_t 
 */
uint8_t TempSensorMAX31855::InitializeReadContinuous(unsigned long duration_ms)
{
    _start_time = millis();
    _end_time = _start_time + duration;
    return ERROR_OKAY;
}

/**
 * @brief check if end time is reached
 * 
 * @return uint8_t 
 */
uint8_t TempSensorMAX31855::CheckEndTime()
{
    if (millis() >= _end_time)
    {
        return TIME_EXPIRED;
    }
    return TIME_AVAILABLE;
}