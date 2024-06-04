#include "arc_lighter.h"

/**
 * @brief Construct a new ArcLighter:: ArcLighter object
 * 
 * @param enable 
 * @param charger
 */
ArcLighter::ArcLighter(uint8_t enable, uint8_t charger)
{
    this->_enable_pin = enable;
    this->_charger_pin = charger;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t ArcLighter::Connect()
{
    pinMode(_enable_pin,OUTPUT);
    digitalWrite(_enable_pin,LOW);
    pinMode(_charger_pin,OUTPUT);
    digitalWrite(_charger_pin,LOW);
    // Serial.print("Charger  pin: "); Serial.println(_enable_pin);
    // Serial.print("Enable  pin: "); Serial.println(_charger_pin);
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @param time_ms 
 * @return uint16_t 
 */
uint16_t ArcLighter::Ignite(uint32_t time_ms = DEFAULT_IGNITION_TIME_MS)
{
    this->Uncharging();
    delay(10);
    // Serial.println("Stop charging, ignite");
    _ignition_duration = time_ms;
    _status = STATUS_ON;
    _ignition_time = millis();
    digitalWrite(_enable_pin,HIGH);
    // delay(time_ms);
    // digitalWrite(_enable_pin,LOW);
    // Serial.println("Stop ignition, charging");
    // delay(100);
    // this->Charging();
    return RESP_GENERAL_FEEDBACK_VOID;
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t ArcLighter::Charging()
{
    digitalWrite(_charger_pin, HIGH);
}

/**
 * @brief 
 * 
 * @return uint16_t 
 */
uint16_t ArcLighter::Uncharging()
{
    digitalWrite(_charger_pin, LOW);
}

uint16_t ArcLighter::CheckStop()
{
    if (millis() - _ignition_time >= _ignition_duration && _status == STATUS_ON)
    {
        digitalWrite(_enable_pin,LOW);
        delay(10);
        this->Charging();
        _status = STATUS_OFF;
        return RESP_GENERAL_FEEDBACK_VOID;
    }
    return RESP_GENERAL_FEEDBACK_WAITING;
}