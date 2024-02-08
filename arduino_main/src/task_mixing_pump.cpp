#include "task_mixing_pump.h"
static void DispenseTimerCallback(TimerHandle_t timer_mixing_pump);

void PumpController(PumpMasterflex* pump, cmd_t* command, resp_t* response)
{
    current_pump = pump;
    if (command->cmd_id.equals(CMD_MIX_PUMP_STOP) || command->cpnt == CPNT_ALL)
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->Stop(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_START))
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->Start(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_DIRECTION))
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->SetDirection(command->operand); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_GET_SPEED))
    {
        response->resp_id = RESP_ID_DATA;
        response->data = pump->GetSpeed(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_SET_SPEED))
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->SetSpeed(command->operand); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_GET_SETTING))
    {
        response->resp_id = RESP_ID_DATA;
        response->data = pump->GetSpeedSetting(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_SET_MAX))
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->SetMaxSpeed(command->operand); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_SET_MIN))
    {
        response->resp_id = RESP_ID_SUCCESS;
        response->data = pump->SetMinSpeed(command->operand); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_GET_MAX))
    {
        response->resp_id = RESP_ID_DATA;
        response->data = pump->GetMaxSpeed(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_GET_MIN))
    {
        response->resp_id = RESP_ID_DATA;
        response->data = pump->GetMinSpeed(); return;
    }
    if (command->cmd_id.equals(CMD_MIX_PUMP_DISPENSE_UL))
    {
        response->resp_id = RESP_ID_PUMP_DISP_START;
        response->data = pump->GetDispenseTime(command->operand);
        PumpDispenseByTime(response->data);
        return;
    }
    // Serial.println("No matching command");
    response->resp_id = RESP_ID_FAILED;
}

void PumpDispenseByTime(int32_t time_ms)
{
    if (timer_created){
        timer_mixing_pump = xTimerCreate("mixing_pump_dispense", time_ms/portTICK_PERIOD_MS, pdFALSE, 0, DispenseTimerCallback);
        timer_created = true;
    }
    else {
        xTimerChangePeriod(timer_mixing_pump, time_ms/portTICK_PERIOD_MS, 100);
    }
    xTimerStart(timer_mixing_pump,10);
    current_pump->Start();
}

static void DispenseTimerCallback(TimerHandle_t timer_mixing_pump)
{
    current_pump->Stop();
    xTimerDelete(timer_mixing_pump,10);
}