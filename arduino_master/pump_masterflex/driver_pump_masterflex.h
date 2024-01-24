/**
 * PumpMasterflex.h - a library to control pump Masterflex L/S 775200
 * Created by Kevin Tieu, October 31, 2023
*/

#ifndef DRIVER_PUMP_MASTERFLEX_H
#define DRIVER_PUMP_MASTERFLEX_H

#include "Arduino.h"
// arduino constant
#define ARDUINO_PWM 255
#define ARDUINO_ANALOG 1023
#define ARDUINO_VCC 5

// Define state
#define PUMP_START LOW
#define PUMP_STOP  HIGH
#define DIR_CW     HIGH
#define DIR_CCW    LOW
#define PRIME_ON   LOW
#define PRIME_OFF  HIGH
#define PIPE_PRIMED 1
#define PIPE_EMPTY  0

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

typedef struct PumpSpeed {
    float speed_ml_min;
    float max_speed;
    float min_speed;
} PumpSpeed_t;

typedef struct MasterflexDB25Pins {
    uint8_t start_stop_pin;
    uint8_t direction_pin;
    uint8_t prime_pin;
    uint8_t voltage_in_pin;
    uint8_t voltage_out_pin;
} MasterflexDB25Interface_t;

typedef struct TubeProperties {
    uint8_t size;
    float max_speed;
    float min_speed;
} Tube_t;

class PumpMasterflex {
    private:
        MasterflexDB25Interface_t _pins;
        uint8_t _state_op = PUMP_STOP;
        uint8_t _state_dir = DIR_CW;
        uint8_t _state_prime = PRIME_OFF;
        Tube_t _tube;
        PumpSpeed_t _speed_control;
        float _max_voltage;
        float _min_voltage;
        int32_t _pipe_vol = 0;
        int32_t _tuning_vol = 0;
        uint8_t _pipe_state = PIPE_EMPTY;
    public:
        PumpMasterflex(MasterflexDB25Interface_t pins);
        bool Connect();
        bool Start();
        bool Stop();
        uint8_t GetOpState();
        bool SetDirection(int32_t direction);
        bool ChangeDirection();
        uint8_t GetDirection();
        bool PrimeStart();
        bool PrimeStop();
        bool Prime(int32_t duration_ms);
        uint8_t GetPrimeState();
        bool SetTubeSize(int32_t size);
        uint8_t GetTubeSize();
        bool SetMaxSpeed(int32_t speed);
        bool SetMinSpeed(int32_t speed);
        int32_t GetMaxSpeed();
        int32_t GetMinSpeed();
        bool SetSpeed(int32_t speed_ul_min);
        int32_t GetSpeedSetting();
        int32_t GetSpeed();
        bool SetMaxVoltageLevel(int32_t voltage_max);
        bool SetMinVoltageLevel(int32_t voltage_min);
        bool Dispense(int32_t amount_ul);
        bool PipeSetVol(int32_t vol_ul);
        bool PipeSetState(uint8_t state);
        bool SetTuningVol(int32_t amount_ul);
};

#endif
