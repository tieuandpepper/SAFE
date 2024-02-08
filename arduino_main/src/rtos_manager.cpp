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
    cmd_t command;
    resp_t response;
    response.cpnt = CPNT_MIX_PUMP;
    MasterflexDB25Interface_t pump_interface {
        .start_stop_pin  = PIN_MIXING_PUMP_REMOTE_CONTROL,
        .direction_pin   = PIN_MIXING_PUMP_CLOCKWISE_CONTROL,
        .prime_pin       = PIN_MIXING_PUMP_PRIME_CONTROL,
        .voltage_in_pin  = PIN_MIXING_PUMP_SPEED_CONTROL,
        .voltage_out_pin = PIN_MIXING_PUMP_SPEED_FEEDBACK,
    };
    PumpMasterflex mixing_pump = PumpMasterflex(pump_interface);

    // xQueueCreate(QueueLength - number of items, ItemSize - number of bytes)
    queue_cmd_mixing_pump = xQueueCreate(10, sizeof(cmd_t));
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

    LOG_DEBUG(LOG_SERV_USER, "mixing pump created");

    while (1)
    {
        // take semaphore/mutex
        // Do task
        if (xQueueReceive(queue_cmd_mixing_pump, &command, (TickType_t) 10))
        {
            response.cmd_id = command.cmd_id;
            PumpController(&mixing_pump, &command, &response);
            // LOG_DEBUG(LOG_SERV_USER, "mixing pump");
        }
        // give back semaphore/mutex
    }
}

void TaskMixer(void * pvParameters)
{
    mixer_t mixer;
    // xQueueCreate(QueueLength - number of items, ItemSize - number of bytes)
    queue_cmd_mixer = xQueueCreate(3, sizeof(cmd_t));
    mixer.pin = PIN_MIXER_ENABLE;
    pinMode(mixer.pin,OUTPUT);
    digitalWrite(mixer.pin, LOW);
    LOG_DEBUG(LOG_SERV_USER, "mixer created");
    while (1)
    {
        // take semaphore/mutex
        // Do task
        LOG_DEBUG(LOG_SERV_USER, "mixer");
        vTaskDelay(500/portTICK_PERIOD_MS);
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
    cmd_t command;
    resp_t response;
    uint32_t i;

    qlist_t queue_list[] = 
    {
        {CPNT_MIX_PUMP, queue_cmd_mixing_pump},
        {CPNT_MIXER, queue_cmd_mixer},
    };
    uint32_t queue_count = sizeof(queue_list)/sizeof(qlist_t);
    while (1)
    {
        // Get each command from buffer and parse it to the command structure for easy process
        if (GetCommand(&command) != CMD_NOTHING)
        {
            response.resp_id = RESP_ID_INVALID;
            LOG_DEBUG(LOG_SERV_USER, "COMPONENT=" << command.cpnt << " | command_id=" << command.cmd_id << " | operand=" << command.operand);
            // send the command struct to the command queue to be digested by other tasks
            for (i = 0; i < queue_count; ++i)
            {
                if ((command.cpnt == queue_list[i].receiver) || (command.cpnt == CPNT_ALL))
                {
                    if (xQueueSend(queue_list[i].queue, (void*) &command, (TickType_t) 10) != pdPASS)
                    {
                        LOG_ERROR(LOG_SERV_RTOS, "Fail to send a command to " << command.cpnt);
                    }
                    response.resp_id = RESP_ID_RECEIVED;
                }
            }
            response.cmd_id = command.cmd_id;
            response.cpnt = command.cpnt;
            response.data = 0;
            if (xQueueSend(queue_response, (void*) &response, (TickType_t) 10) != pdPASS)
            {
                LOG_ERROR(LOG_SERV_RTOS, "Fail to send a response");
            }
        }
        vTaskDelay(20/portTICK_PERIOD_MS);
    }
}

void TaskResponseProcess(void * pvParameters)
{
    // initialization
    resp_t response;
    queue_response = xQueueCreate(10, sizeof(resp_t));
    while (1)
    {
        if (xQueueReceive(queue_response, &response, (TickType_t) 10) == pdTRUE)
        {
            SendResponse(response);
        }
    }
}