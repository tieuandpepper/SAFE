// /**
//  * @file task_controller.h
//  * @author Phat (Kevin) Tieu (phattieuthien@gmail.com)
//  * @brief contain all the task functions
//  * @version 0.1
//  * @date 2024-01-23
//  * 
//  * @copyright Copyright (c) 2024
//  * 
//  */

// #ifndef TASK_CONTROLLER_H
// #define TASK_CONTROLLER_H

// #include "Arduino.h"
// #include <Arduino_FreeRTOS.h>
// #include "arduino_pinout.h"
// #include "pump_masterflex/driver_pump_masterflex.h"
// #include "mixer/task_mixer.h"

// // SemaphoreHandle_t semaphore_mixing_pump;
// // SemaphoreHandle_t semaphore_mixer;
// // SemaphoreHandle_t mutex_mixing_vessel;

// MasterflexDB25Interface_t mixing_pump_interface {
//   .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
//   .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
//   .prime_pin       = PIN_MIXING_PUMP_PRIME_CONTROL,
//   .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
//   .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
// };

// void TaskMixingPump(void *pvParameters);
// void TaskMixer(void *pvParameters);
// void TaskTransferPump(void *pvParameters);
// void TaskTestMonitor(void *pvParameters);
// void TaskLighter(void *pvParameters);
// void TaskCommandProcess(void *pvParameters);
// void TaskResponseProcess(void *pvParameters);

// #endif