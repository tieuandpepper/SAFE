#include "device_controller.h"

/**
 * @brief control Masterflex pump by processing a command
 * 
 * @param pump object stores info and command for the pump
 * @param command command from user
 * @return resp_t response from the pump driver
 */
resp_t MasterflexPumpController(PumpMasterflex* pump, cmd_t command)
{
  resp_t response;
  uint32_t data;
  Serial.println("Mixpump received a command");
  response.source = DEVICE_MASTERFLEXPUMP;
  response.type = RESP_TYPE_VALID;
  response.data = "";
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  if (command.instruction.equals(MIXPUMP_START))
  {
    response.error_code = pump->Start();
    return response;
  }
  if (command.instruction.equals(MIXPUMP_STOP))
  {
    response.error_code = pump->Stop();
    return response;
  }
  if (command.instruction.equals(MIXPUMP_CW))
  {
    response.error_code = pump->SetDirection(DIR_CW);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_CCW))
  {
    response.error_code = pump->SetDirection(DIR_CCW);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_SETSPEED))
  {
    response.error_code = pump->SetSpeed(command.parameter);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_DISPENSE))
  {
    response.error_code = pump->Dispense(command.parameter);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_SETMAXSPEED))
  {
    response.error_code = pump->SetMaxSpeed(command.parameter);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_SETMINSPEED))
  {
    response.error_code = pump->SetMinSpeed(command.parameter);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_PIPESETVOL))
  {
    response.error_code = pump->PipeSetVol(command.parameter);
    return response;
  }
  if (command.instruction.equals(MIXPUMP_GETSPEED))
  {
    response.error_code = pump->GetSpeed(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(MIXPUMP_GETMAXSPEED))
  {
    response.error_code = pump->GetMaxSpeed(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(MIXPUMP_GETMINSPEED))
  {
    response.error_code = pump->GetMinSpeed(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(MIXPUMP_GETSPEEDSETTING))
  {
    response.error_code = pump->GetSpeedSetting(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }

  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}

/**
 * @brief control mixer by processing command
 * 
 * @param mixer mixer object
 * @param command 
 * @return resp_t response
 */
resp_t MixerController(Mixer * mixer, cmd_t command)
{
  resp_t response;
  Serial.println("Mixer received a command");
  response.source = DEVICE_MIXER;
  response.type = RESP_TYPE_VALID;
  response.data = "";
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  if (command.instruction.equals(MIXER_START))
  {
    response.error_code = mixer->Start();
    return response;
  }
  if (command.instruction.equals(MIXER_STOP))
  {
    response.error_code = mixer->Stop();
    return response;
  }
  if (command.instruction.equals(MIXER_RUN))
  {
    response.error_code = mixer->Run(command.parameter);
    return response;
  }
  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}

/**
 * @brief control temperature sensor
 * 
 * @param sensor sensor object (must be passed by reference)
 * @param command 
 * @return resp_t response
 */
resp_t TempSensorController(TempSensorMAX31855 * sensor, cmd_t command)
{
  resp_t response;
  float data = 0.0;
  response.source = DEVICE_TEMPSENSOR;
  response.data = "";
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  response.type = RESP_TYPE_VALID;

  // check for background/periodic tasks
  if (sensor->DurationFlagOn() && command.target == DEVICE_NONE)
  {
    // Serial.println("read by duration");
    response.error_code = sensor->ReadSensorbyDuration(&data);
    if (response.error_code == RESP_TEMPSENSOR_READ_TIMER_WAITING)
    {
      response.type = RESP_TYPE_NOTHING;
    }
    else {
      response.data = (uint32_t)(data*1000);
      response.type = RESP_TYPE_FEEDBACK;
    }
    return response;
  }
  if (sensor->PeriodFlagOn() && command.target == DEVICE_NONE)
  {
    // Serial.println("Read by period");
    response.error_code = sensor->ReadSensorbyPeriod(&data);
    if (response.error_code == RESP_TEMPSENSOR_READ_TIMER_WAITING)
    {
      response.type = RESP_TYPE_NOTHING;
    }
    else {
      response.data = (uint32_t)(data*1000);
      response.type = RESP_TYPE_FEEDBACK;
    }
    return response;
  }
  // no periodic task and no active command
  if (command.target == DEVICE_NONE)
  {
    response.source = DEVICE_NONE;
    return response;
  }
  // run active command
  // Serial.println("Temp sensor received a command");
  if (command.instruction.equals(TEMPSENSOR_READ_ONCE))
  {
    response.error_code = sensor->ReadSensor(&data);
    // data = 123456.123456;
    response.data = String(data*1000);
    response.type = RESP_TYPE_FEEDBACK;
    // Serial.println(data);
    // Serial.println(response.data);
    return response;
  }
  if (command.instruction.equals(TEMPSENSOR_READ_DURATION_MS))
  {
    response.error_code = sensor->InitializeReadDuration(command.parameter);
    return response;
  }
  if (command.instruction.equals(TEMPSENSOR_READ_PERIOD_MS))
  {
    response.error_code = sensor->InitializeReadPeriod(command.parameter);
    return response;
  }
  if (command.instruction.equals(TEMPSENSOR_READ_STOP))
  {
    response.error_code = sensor->StopRead();
    return response;
  }

  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}

/**
 * @brief control rotary valve from commands
 * 
 * @param valve rotary valve object
 * @param command 
 * @return resp_t response object
 */
resp_t RotaryValveController(RotaryValve * valve, cmd_t command)
{
  resp_t response;
  uint32_t data;

  Serial.println("Rotary valve received a command");
  response.source = DEVICE_ROTARYVALVE;
  response.type = RESP_TYPE_VALID;
  response.data = "";
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  if (command.instruction.equals(ROTARYVALVE_SET_ADDR))
  {
    response.error_code = valve->FactorySetAddr(command.parameter);
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_SET_BAUD_RATE))
  {
    response.error_code = valve->FactorySetBaudRate(command.parameter);
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_FACTORY_RESET))
  {
    response.error_code = valve->FactorySetAutoReset(command.parameter);
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_GET_ADDR))
  {
    response.error_code = valve->QueryAddress(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_GET_BAUD_RATE))
  {
    response.error_code = valve->QueryBaudRate(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_GET_MOTOR_STATUS))
  {
    response.error_code = valve->QueryMotorStatus(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_GET_VERSION))
  {
    response.error_code = valve->QueryCurrVersion(&data);
    response.data = String(data);
    response.type = RESP_TYPE_FEEDBACK;
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_RESET_ORIGIN))
  {
    response.error_code = valve->ActionResetOrigin();
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_MOVE_AUTO))
  {
    response.error_code = valve->ActionMoveAuto(command.parameter);
    return response;
  }
  if (command.instruction.equals(ROTARYVALVE_STOP))
  {
    response.error_code = valve->ActionStop();
    return response;
  }
  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}

/**
 * @brief control lighter from command
 * 
 * @param ArcLighter lighter object
 * @param command 
 * @return resp_t response
 */
resp_t ArcLighterController(ArcLighter * lighter, cmd_t command)
{
  resp_t response;
  Serial.println("Lighter received a command");
  response.source = DEVICE_LIGHTER;
  response.data = "";
  response.type = RESP_TYPE_VALID;
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  if (command.instruction.equals(LIGHTER_IGNITE))
  {
    if (command.parameter <= 0)
    {
      response.error_code = lighter->Ignite();
    }
    else 
    {
      // Serial.println("Ignite");
      response.error_code = lighter->Ignite(command.parameter);
    }
    return response;
  }
  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}

resp_t EZOPumpController(EZOPump * pump, cmd_t command)
{
  resp_t response;
  String data;
  Serial.println("EZO Pump received a command");
  response.source = DEVICE_EZOPUMP;
  response.data = "";
  response.type = RESP_TYPE_VALID;
  response.error_code = RESP_GENERAL_FEEDBACK_VOID;
  if (command.instruction.equals(EZOPUMP_DISPENSE_CONTINUOUS))
  {
    response.error_code = pump->DispenseContinuous(command.parameter);
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DISPENSE_AMOUNT))
  {
    response.error_code = pump->DispenseVolume((float)command.parameter/1000);
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DISPENSE_PAUSE))
  {
    response.error_code = pump->DispensePause();
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DISPENSE_STOP))
  {
    response.error_code = pump->DispenseStop(&data);
    response.data = data;
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DISPENSE_READING))
  {
    response.error_code = pump->SingleReport(&data);
    response.data = data;
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DISPENSE_STATUS))
  {
    response.error_code = pump->DispenseStatus(&data);
    response.data = data;
    return response;
  }
  if (command.instruction.equals(EZOPUMP_CALIBRATE))
  {
    response.error_code = pump->Calibrate((float)command.parameter/1000);
    return response;
  }
  if (command.instruction.equals(EZOPUMP_CALIBRATE_RESET))
  {
    response.error_code = pump->CalibrateReset();
    return response;
  }
  if (command.instruction.equals(EZOPUMP_VOLTAGE))
  {
    response.error_code = pump->VoltageCheck(&data);
    response.data = data;
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DEVICE_INFO))
  {
    response.error_code = pump->DeviceInfo(&data);
    response.data = data;
    return response;
  }
  if (command.instruction.equals(EZOPUMP_DIRECTION_CHANGE))
  {
    response.error_code = pump->DirectionChange();
    return response;
  }
  response.type = RESP_TYPE_INVALID;
  response.error_code = RESP_GENERAL_ERROR_INVALID_INSTRUCTION;
  return response;
}