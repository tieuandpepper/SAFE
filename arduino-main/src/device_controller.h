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

resp_t AllDeviceController(cmd_t command);
resp_t MasterflexPumpController(PumpMasterflex* pump, cmd_t command);
resp_t MixerController(Mixer * mixer, cmd_t command);
resp_t TempSensorController(TempSensorMAX31855 * sensor, cmd_t command);
resp_t RotaryValveController(RotaryValve * valve, cmd_t command);
resp_t LighterController(Lighter * lighter, cmd_t command);

#endif