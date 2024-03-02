/**
 * @file rotary_valve.ino
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <SoftwareSerial.h>
#include "rotary_valve.h"

#define ROT_VALVE_RX   9
#define ROT_VALVE_TX   10

// SoftwareSerial rotary_valve(ROT_VALVE_RX, ROT_VALVE_TX, true);
RotaryValve rotary_valve(ROT_VALVE_RX, ROT_VALVE_TX, 16, 9600);

int ret;
void setup() {
    Serial.begin(9600);
    rotary_valve.initialize();
    // ret = rotary_valve.SendCommandTX(ACT_FUNC_ROTATE_AUTO,0x0001);
    // Serial.println("Initialization completed");
    // byte message[] = {0xCC, 0x00, 0x44, 0x07, 0x00 , 0xDD, 0xF4, 0x01};

    // rotary_valve.write(message, sizeof(message));
    // Serial.println(ret);
}
// String response;
// byte a; 
uint32_t resp_val = 0;

void loop() {
    ret = rotary_valve.QueryBaudRate(&resp_val);
    // ret = rotary_valve.SendCommandTX(QUERY_FUNC_CURRENT_VERSION);
    Serial.print("Return "); Serial.println(ret, HEX);
    Serial.print("Response value: "); Serial.println(resp_val, HEX);

    while (1)
    {}



    // delay(1000);
    // while (!rotary_valve.available())
    // {
    //     while (rotary_valve.available())
    //     {
    //         ret = rotary_valve.ReceiveResponse();
    //         Serial.print("Reading response: "); Serial.println(ret);
    //         if (ret == false)
    //         {
    //             Serial.println("Failed");
    //             break;
    //         }
    //         response = rotary_valve.PrintResponse();
    //         Serial.print(response);
    //         Serial.println("done");
    //         return;
    //     }
    //     delay(50);
    // }
    // while(1)
    // {}
}