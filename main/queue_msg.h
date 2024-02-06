/**
 * @file queue_msg.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef QUEUE_MSG_H
#define QUEUE_MSG_H

// Component
#define CPNT_MIX_PUMP           0x01
#define CPNT_MIXER              0x02
#define CPNT_ROTARY_VALVE       0x03
#define CPNT_LIGHTER            0x04
#define CPNT_TEMP_SENSOR        0x05
#define CPNT_TRANSFER_PUMP      0x06


// Mixing Pump command
#define CMD_MIX_PUMP_STOP           ((CPNT_MIX_PUMP << 8) + 0x00)
#define CMD_MIX_PUMP_START          ((CPNT_MIX_PUMP << 8) + 0x01)
#define CMD_MIX_PUMP_DIRECTION      ((CPNT_MIX_PUMP << 8) + 0x02)
#define CMD_MIX_PUMP_GET_SPEED      ((CPNT_MIX_PUMP << 8) + 0x03)
#define CMD_MIX_PUMP_SET_SPEED      ((CPNT_MIX_PUMP << 8) + 0x04)
#define CMD_MIX_PUMP_GET_SETTING    ((CPNT_MIX_PUMP << 8) + 0x05)
#define CMD_MIX_PUMP_SET_MAX        ((CPNT_MIX_PUMP << 8) + 0x06)
#define CMD_MIX_PUMP_SET_MIN        ((CPNT_MIX_PUMP << 8) + 0x07)
#define CMD_MIX_PUMP_GET_MAX        ((CPNT_MIX_PUMP << 8) + 0x08)
#define CMD_MIX_PUMP_GET_MIN        ((CPNT_MIX_PUMP << 8) + 0x09)
#define CMD_MIX_PUMP_SET_PIPE_VOL   ((CPNT_MIX_PUMP << 8) + 0x0A)

// Mixer command
#define CMD_MIXER_STOP              ((CPNT_MIXER << 8) + 0x00)
#define CMD_MIXER_START             ((CPNT_MIXER << 8) + 0x01)

// Temperature sensor
#define CMD_TEMP_SENSOR_READ        ((CPNT_TEMP_SENSOR << 8) + 0x01)
#define CMD_TEMP_SENSOR_BEGIN       ((CPNT_TEMP_SENSOR << 8) + 0x02)

// Lighter
#define CMD_LIGHTER_STOP            ((CPNT_LIGHTER << 8) + 0x00)
#define CMD_LIGHTER_RUN             ((CPNT_LIGHTER << 8) + 0x01)


#endif