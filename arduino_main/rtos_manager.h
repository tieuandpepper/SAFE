/**
 * @file rtos_manager.h
 * @author your name (you@domain.com)
 * @brief all tasksrelated to rtos operations are contained here
 * @version 0.1
 * @date 2024-01-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RTOS_MANAGER_H
#define RTOS_MANAGER_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <DebugLog.h>
#include "arduino_pinout.h"
#include "src/pump_masterflex.h"
#include "src/mixer_generic.h"
#include "task_comm.h"

#define TASK_PRIORITY_COMMUNICATION       2
#define TASK_PRIORITY_PROCESSING          3
#define TASK_PRIORITY_MONITORING          4

extern SemaphoreHandle_t mutex_mixing_vessel;
extern SemaphoreHandle_t mutex_test_chamber;
extern QueueHandle_t queue_cmd;

void TaskMixingPump(void * pvParameters);
void TaskMixer(void * pvParameters);
void TaskTransferPump(void * pvParameters);
void TaskTestMonitor(void * pvParameters);
void TaskLighter(void * pvParameters);
void TaskReceiveCommand(void * pvParameters);
void TaskSendResponse(void * pvParameters);

#endif
