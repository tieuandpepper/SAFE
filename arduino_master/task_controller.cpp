// /**
//  * @file task_controller.cpp
//  * @author your name (you@domain.com)
//  * @brief 
//  * @version 0.1
//  * @date 2024-01-23
//  * 
//  * @copyright Copyright (c) 2024
//  * 
//  */

// #include "task_controller.h"

// /// @brief initialize and control the mixing pump - masterflex
// void TaskMixingPump(void *pvParameters)
// {
//     PumpMasterflex mixing_pump = PumpMasterflex(mixing_pump_interface);
//     mixing_pump.Connect();
//     mixing_pump.Stop();
//     mixing_pump.PrimeStop();
//     mixing_pump.SetDirection(DIR_CW);
//     mixing_pump.SetTubeSize(14);
//     mixing_pump.SetMaxVoltageLevel(5000);
//     mixing_pump.SetMinVoltageLevel(100);
//     mixing_pump.SetMinSpeed(0);
//     mixing_pump.SetMaxSpeed(37700);
//     mixing_pump.SetSpeed(14000);
//     mixing_pump.PipeSetVol(1700);

//     semaphore_mixing_pump = xSemaphoreCreateBinary();
//     if (semaphore_mixing_pump != NULL) {
//     Serial.println("Semaphore for mixing pump...CREATED SUCCESSFULLY");
//     }

//     mutex_mixing_vessel = xSemaphoreCreateMutex();
//     if (mutex_mixing_vessel != NULL) {
//     Serial.println("Mutex for mixing vessel...CREATED SUCCESSFULLY");
//     }

//     while (1)
//     {
//         if (xSemaphoreTake(mutex_mixing_vessel, 10) == pdFALSE ||
//             xSemaphoreTake(semaphore_mixing_pump, 10) == pdFALSE)
//         {
//             continue;
//         }
//         // Do task
//         xSemaphoreGive(mutex_mixing_vessel);
//     }
// }

// /// @brief 
// void TaskMixer(void *pvParameters)
// {
//     mixer_t mixer;
//     mixer.pin = PIN_MIXER_ENABLE;
//     pinMode(mixer.pin,OUTPUT);
//     digitalWrite(mixer.pin, LOW);
// }

// /// @brief 
// void TaskTransferPump(void *pvParameters)
// {

// }

// /// @brief 
// void TaskTestMonitor(void *pvParameters)
// {

// }

// /// @brief 
// void TaskLighter(void *pvParameters)
// {

// }

// /// @brief 
// void TaskCommandProcess(void *pvParameters)
// {

// }

// /// @brief 
// void TaskResponseProcess(void *pvParameters)
// {

// }