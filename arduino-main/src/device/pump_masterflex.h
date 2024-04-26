/**
 * PumpMasterflex.h - a library to control pump Masterflex L/S 775200
 * Created by Kevin Tieu, October 31, 2023
*/

#ifndef PumpMasterflex_h
#define PumpMasterflex_h

#include "Arduino.h"
#include "response_error_code.h"
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
        Tube_t _tube;
        PumpSpeed_t _speed_control;
        float _max_voltage;
        float _min_voltage;
        uint32_t _pipe_vol = 0;
        uint32_t _tuning_vol = 0;
        uint8_t _pipe_state = PIPE_EMPTY;
    public:
        PumpMasterflex(MasterflexDB25Interface_t pins);
        uint16_t Connect();
        uint16_t Start();
        uint16_t Stop();
        uint16_t GetOpState();
        uint16_t SetDirection(uint32_t direction);
        uint16_t ChangeDirection();
        uint16_t GetDirection();
        uint16_t SetTubeSize(uint32_t size);
        uint16_t GetTubeSize();
        uint16_t SetMaxSpeed(uint32_t speed);
        uint16_t SetMinSpeed(uint32_t speed);
        uint16_t GetMaxSpeed(uint32_t * speed);
        uint16_t GetMinSpeed(uint32_t * speed);
        uint16_t SetSpeed(uint32_t speed_ul_min);
        uint16_t GetSpeedSetting(uint32_t * speed);
        uint16_t GetSpeed(uint32_t * speed);
        uint16_t SetMaxVoltageLevel(uint32_t voltage_max);
        uint16_t SetMinVoltageLevel(uint32_t voltage_min);
        uint16_t Dispense(uint32_t amount_ul);
        uint16_t PipeSetVol(uint32_t vol_ul);
        uint16_t PipeSetState(uint32_t state);
        uint16_t SetTuningVol(uint32_t amount_ul);
};

#endif
