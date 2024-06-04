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
    _start_time = 0;
    _end_time = 0;
    _period_time = 0;
    _read_flag = FLAG_READ_ONCE;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::Connect()
{
    this->begin();
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief read sensor, return the reading and update the data buffer
 * 
 * @param data 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::ReadSensor(float * data)
{
    (*data) = this->readCelsius();
    // Serial.print("ReadTemp="); Serial.println(*data);
    if ((*data) == NAN)
    {
        // Serial.print("ReadTemp="); Serial.println(*data);
        return RESP_TEMPSENSOR_ERR_SENSOR;
    }
    return RESP_GENERAL_FEEDBACK_SUCCESS;
}

/**
 * @brief initialize continuous read mode until time run out
 * @note this will overflow after about 49.7 days 
 * 
 * @param duration
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::InitializeReadDuration(uint32_t duration_ms)
{
    _start_time = millis();
    _end_time = _start_time + duration_ms;
    _read_flag = FLAG_READ_DURATION;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param period_ms 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::InitializeReadPeriod(uint32_t period_ms)
{
    _start_time = millis();
    _period_time = period_ms;
    _end_time = _start_time + _period_time;
    _read_flag = FLAG_READ_PERIOD;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief check if end time is reached
 * 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::CheckEndTime()
{
    if (millis() >= _end_time)
    {
        return TIME_EXPIRED;
    }
    return TIME_AVAILABLE;
}

/**
 * @brief read by a duration (no repeat)
 * 
 * @param data 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::ReadSensorbyDuration(float* data)
{
    if (_read_flag != FLAG_READ_DURATION)
    {
        return RESP_GENERAL_FEEDBACK_VOID;
    }
    if (this->CheckEndTime() == TIME_EXPIRED)
    {
        _read_flag = FLAG_READ_ONCE;
        return this->ReadSensor(data);
    }
    return RESP_GENERAL_FEEDBACK_WAITING;
}

/**
 * @brief read for every period of time (repeatedly)
 * 
 * @param data 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::ReadSensorbyPeriod(float* data)
{
    if (_read_flag != FLAG_READ_PERIOD)
    {
        return RESP_GENERAL_FEEDBACK_VOID;
    }
    if (this->CheckEndTime() == TIME_EXPIRED)
    {
        _end_time = millis() + _period_time;
        return this->ReadSensor(data);
    }
    return RESP_GENERAL_FEEDBACK_WAITING;
}

/**
 * @brief Stop read by duration/period
 * 
 * @return uint16_t 
 */
uint16_t TempSensorMAX31855::StopRead()
{
    _read_flag = FLAG_READ_ONCE;
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TempSensorMAX31855::DurationFlagOn()
{
    if (_read_flag == FLAG_READ_DURATION)
    {
        return true;
    }
    return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TempSensorMAX31855::PeriodFlagOn()
{
    if (_read_flag == FLAG_READ_PERIOD)
    {
        return true;
    }
    return false;
}