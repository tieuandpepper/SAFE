/**
 * @file rtos_manager.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "rtos_manager.h"

void TaskMixingPump(void * pvParameters)
{
    MasterflexDB25Interface_t pump_interface {
        .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
        .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
        .prime_pin       = PIN_MIXING_PUMP_PRIME_CONTROL,
        .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
        .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
    };
    PumpMasterflex mixing_pump = PumpMasterflex(pump_interface);

    mixing_pump.Connect();
    mixing_pump.Stop();
    mixing_pump.PrimeStop();
    mixing_pump.SetDirection(DIR_CW);
    mixing_pump.SetTubeSize(14);
    mixing_pump.SetMaxVoltageLevel(5000);
    mixing_pump.SetMinVoltageLevel(100);
    mixing_pump.SetMinSpeed(0);
    mixing_pump.SetMaxSpeed(37700);
    mixing_pump.SetSpeed(14000);
    mixing_pump.PipeSetVol(1700);
    LOG_INFO("mixing pump created");

    while (1)
    {
        // take semaphore/mutex
        // Do task
        LOG_INFO("mixing pump");
        // give back semaphore/mutex
    }
}

void TaskMixer(void * pvParameters)
{
    mixer_t mixer;
    mixer.pin = PIN_MIXER_ENABLE;
    pinMode(mixer.pin,OUTPUT);
    digitalWrite(mixer.pin, LOW);
    LOG_INFO("mixer created");
    while (1)
    {
        // take semaphore/mutex
        // Do task
        LOG_INFO("mixer");

        // give back semaphore/mutex
    }
}

/**
 * @brief Task control for transfer pump
 * @note Not available yet
 * 
 * @param pvParameters 
 */
void TaskTransferPump(void * pvParameters)
{
    // initialization

    // while (1)
    // {
    //     // take semaphore/mutex
    //     // Do task
    //     // give back semaphore/mutex
    // }
}

/**
 * @brief monitor the test chamber, using sensors and camera (if needed)
 * 
 * @param pvParameters 
 */
void TaskTestMonitor(void * pvParameters)
{
    // initialization

    // while (1)
    // {
    //     // take semaphore/mutex
    //     // Do task
    //     // give back semaphore/mutex
    // }
}

/**
 * @brief control the ignition lighter in the test chamber
 * 
 * @param pvParameters 
 */
void TaskLighter(void * pvParameters)
{
    // initialization

    // while (1)
    // {
    //     // take semaphore/mutex
    //     // Do task
    //     // give back semaphore/mutex
    // }
}

void TaskReceiveCommand(void * pvParameters)
{
    // initialization
    // xQueueCreate(QueueLength - number of items, ItemSize - number of bytes)
    queue_pc_cmd = xQueueCreate(10, sizeof(int));

    while (1)
    {
        // take semaphore/mutex
        // Do task
        // give back semaphore/mutex
    }
}

void TaskResponseProcess(void * pvParameters)
{
    // initialization

    while (1)
    {
        // take semaphore/mutex
        // Do task
        // give back semaphore/mutex
    }
}