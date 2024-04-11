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
    _read_flag = FLAG_READ_ONCE;
}

/**
 * @brief read sensor, return the reading and update the data buffer
 * 
 * @param data 
 * @return int 
 */
int TempSensorMAX31855::ReadSensor(float * data)
{
    (*data) = _sensor_obj.readCelsius();
    if ((*data) == NAN)
    {
        return RESP_TEMPSENSOR_ERR_SENSOR;
    }
    return RESP_FEEDBACK_SUCCESS;
}

/**
 * @brief initialize continuous read mode until time run out
 * @note this will overflow after about 49.7 days 
 * 
 * @param duration
 * @return int 
 */
int TempSensorMAX31855::InitializeReadContinuous(unsigned long duration_ms)
{
    _start_time = millis();
    _end_time = _start_time + duration_ms;
    _read_flag = FLAG_READ_CONT;
    return RESP_FEEDBACK_VOID;
}

/**
 * @brief check if end time is reached
 * 
 * @return int 
 */
int TempSensorMAX31855::CheckEndTime()
{
    if (millis() >= _end_time)
    {
        return TIME_EXPIRED;
    }
    return TIME_AVAILABLE;
}

/**
 * @brief 
 * 
 * @param data 
 * @return int 
 */
int TempSensorMAX31855::ReadSensorContinous(float* data)
{
    if (_read_flag == FLAG_READ_ONCE)
    {
        return RESP_TEMPSENSOR_CONT_READ_FLAG_OFF;
    }
    if (this->CheckEndTime() == TIME_EXPIRED)
    {
        _read_flag = FLAG_READ_ONCE;
        return RESP_TEMPSENSOR_READ_CONT_DONE;
    }
    return this->ReadSensor(data);
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TempSensorMAX31855::ContinuousFlagOn()
{
    if (_read_flag == FLAG_READ_CONT)
    {
        return true;
    }
    return false;
}