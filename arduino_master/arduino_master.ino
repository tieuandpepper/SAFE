/**
 * @file arduino_master.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino_FreeRTOS.h>
#include "arduino_pinout.h"
#include "task_controller.h"

#define TASK_PRIORITY_COMMUNICATION       2
#define TASK_PRIORITY_PROCESSING          3
#define TASK_PRIORITY_MONITORING          4

/*----------------------------------------------------------------------------------------------------------*/


DFRobot_MAX31855 max31855;

/// @brief Setup/ Initialization. Run first and run ONCE
void setup() {
  Serial.begin(9600);
  
  max31855.begin();

  xTaskCreate(TaskMixingPump,                       // Point to TaskMixingPump function
              "MixingPumpControl",                  // Task name
              128,                                  // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_PROCESSING,             // priority number (0 is lowest-idle)
              NULL);                                // task's handle  

  xTaskCreate(TaskMixer,                            // Point to TaskMixer function
              "MixerControl",                       // Task name
              128,                                  // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_PROCESSING,             // priority number (0 is lowest-idle)
              NULL);                                // task's handle

  xTaskCreate(TaskTransferPump,                     // Point to TaskTransferPump function
              "TransferPumpControl",                // Task name
              128,                                  // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_PROCESSING,             // priority number (0 is lowest-idle)
              NULL);                                // task's handle

  xTaskCreate(TaskTestMonitor,                      // Point to TaskTestMonitor function
              "TestMonitor",                        // Task name
              1024,                                 // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_MONITORING,             // priority number (0 is lowest-idle)
              NULL);                                // task's handle

  xTaskCreate(TaskLighter,                          // Point to TaskLighter function
              "LighterControl",                     // Task name
              128,                                  // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_PROCESSING,             // priority number (0 is lowest-idle)
              NULL);                                // task's handle
              
  xTaskCreate(TaskCommandProcess,                   // Point to TaskCommandProcess function
              "CommandProcess",                     // Task name
              1024,                                 // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_COMMUNICATION,          // priority number (0 is lowest-idle)
              NULL);                                // task's handle

  xTaskCreate(TaskResponseProcess,                  // Point to TaskResponseProcess function
              "ResponseProcess",                    // Task name
              1024,                                 // number of words (32 bits/ 4 bytes) for the task's stack
              NULL,                                 // task input parameter
              TASK_PRIORITY_COMMUNICATION,          // priority number (0 is lowest-idle)
              NULL);                                // task's handle
}

int32_t res;
cmd_t command;
resp_t response;
/// @brief Run after setup(). Will run in loop repeatedly
void loop() {
  // Serial.println("Ready");
  if (GetCommand(&command) == CMD_RECEIVED)
  {
    if (command.target.equals("TEMP"))
    {
      float temp = max31855.readCelsius();
      res = (int32_t)temp;
    }
    else if (command.target.equals(MIXER))
    {
      res = MixerController(&mixer, &command);
    }
    else
    {
      res = PumpController(pump, &command);
    }
    response.source = command.target;
    // Serial.println("READY");
    if (res == CMD_INVALID)
    {
      response.resp_id = RESP_INVALID;
      // Serial.println("Invalid command");
    }
    else 
    {
      // Serial.print("Command return ");
      // Serial.println(res);
      response.resp_id = RESP_VALID;
      response.data = res;
    }
    SendResponse(response);
    command.operand = 0;
    response.data = 0;
  }
  delay(200);
}
