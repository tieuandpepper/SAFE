/**
 * @file task_mixing_pump.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TASK_MIXING_PUMP_H
#define TASK_MIXING_PUMP_H

#include <Arduino_FreeRTOS.h>
#include "driver/pump_masterflex.h"
#include "task_comm.h"
#include <timers.h>

extern TimerHandle_t timer_mixing_pump;

static PumpMasterflex * current_pump;
static bool timer_created = false;

void PumpController(PumpMasterflex* pump, cmd_t* command, resp_t* response);
void PumpDispenseByTime(int32_t time_ms);
#endif