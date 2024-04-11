#ifndef DEVICE_CONTROLLER_H
#define DEVICE_CONTROLLER_H

#include <Arduino.h>
#include "serial_comm.h"
#include "device/mixer.h"
#include "device/pump_masterflex.h"
#include "device/rotary_valve.h"
#include "device/temp_sensor.h"
#include "device/ezo_pump.h"
#include "command_list.h"

int MixPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond);
int MixerController(Mixer * mixer, cmd_t * command, resp_t* respond);
int TempSensorController(TempSensorMAX31855 * sensor, cmd_t * command, resp_t* respond);
int RotaryValveController(RotaryValve * valve, cmd_t * command, resp_t* respond);
int TransferPumpController(PumpMasterflex* pump, cmd_t* command, resp_t* respond);

#endif