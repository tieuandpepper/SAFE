#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include <Arduino.h>
#include "serial_comm.h"
#include "command_list.h"
#include "device/mixer.h"
#include "device/pump_masterflex.h"
#include "device/rotary_valve.h"
#include "device/temp_sensor.h"
#include "device/ezo_pump.h"
#include "device/lighter.h"

void MixPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* response);
void MixerController(Mixer * mixer, cmd_t * command, resp_t* response);
void TempSensorController(TempSensorMAX31855 * sensor, cmd_t * command, resp_t* response);
void RotaryValveController(RotaryValve * valve, cmd_t * command, resp_t* response);
void LighterController(Lighter * lighter, cmd_t* command, resp_t* response);

#endif