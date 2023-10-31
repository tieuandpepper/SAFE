/**
 * PumpMasterflex.h - a library to control pump Masterflex L/S 775200
 * Created by Kevin Tieu, October 31, 2023
*/

#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

// Define state
#define PUMP_ON    LOW
#define PUMP_OFF   HIGH
#define DIR_CW     HIGH
#define DIR_CCW    LOW
#define PRIME_ON   LOW
#define PRIME_OFF  HIGH
// define tube size
#define TUBE_SIZE_13 13
#define TUBE_MAX_SPEED_13  36 // ml/min (600rpm)
#define TUBE_MIN_SPEED_13  0.06 // ml/min (1rpm)
#define TUBE_SIZE_14 14
#define TUBE_MAX_SPEED_14  130 // ml/min (600rpm)
#define TUBE_MIN_SPEED_14  0.22 // ml/min (1rpm)
#define TUBE_SIZE_16 16
#define TUBE_MAX_SPEED_16  480 // ml/min (600rpm)
#define TUBE_MIN_SPEED_16  0.80 // ml/min (1rpm)

// preprocessor expansion to get tube properties
#define concat(a,b) a##b
#define tube_size(s) concat(TUBE_SIZE_, s)
#define tube_max_speed(s) concat(TUBE_MAX_SPEED_,s)
#define tube_min_speed(s) concat(TUBE_MIN_SPEED_,s)

typedef struct PumpSpeed {
    double speed_percent;
    double speed_ml_min;
} PumpSpeed_t;

typedef struct MasterflexDB25Pins {
    uint8_t start_stop_pin;
    uint8_t direction_pin;
    uint8_t prime_pin;
    uint8_t voltage_in_pin;
    uint8_t voltage_out_pin;
} MasterflexDB25Interface_t;

class PumpMasterflex {
    private:
        MasterflexDB25Interface_t _pins;
        uint8_t state_op = PUMP_OFF;
        uint8_t state_dir = DIR_CW;
        uint8_t state_prime = PRIME_OFF;
        uint8_t tube_size;
        PumpSpeed_t speed_control;
        uint32_t max_speed;
        uint32_t min_speed;
        uint32_t max_voltage;
        uint32_t min_voltage;
    public:
        PumpMasterflex(MasterflexDB25Interface_t pins);
        void Connect();
        void Start();
        void Stop();
        void PrimeStart();
        void PrimeStop();
        void Prime(uint64_t duration_ms);
        void SetDirection(uint8_t direction);
        void ChangeDirection();
        void SetTubeSize(uint8_t size);
        void SetVoltageLevel(uint8_t voltage_max, uint8_t voltage_min = 0);
        void SetSpeedPercent(double percent);
        void SetSpeed(double speed_ml_min);
        void SetMaxSpeed(double speed);
        void SetMinSpeed(double speed);
        uint8_t GetOpState();
        uint8_t GetDirection();
        uint8_t GetPrimeState();
        uint8_t GetTubeSize();
        double GetSpeedPercent();
        double GetSpeed();

};

#endif