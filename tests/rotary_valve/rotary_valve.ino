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

#define ROT_VALVE_RX   10
#define ROT_VALVE_TX   9

// SoftwareSerial rotary_valve(ROT_VALVE_RX, ROT_VALVE_TX, true);
RotaryValve rotary_valve(ROT_VALVE_RX, ROT_VALVE_TX, 9600);

int ret;
void setup() {
    Serial.begin(9600);
    rotary_valve.begin(9600);
    ret = rotary_valve.SendCommandTX(ACT_FUNC_ROTATE_AUTO,0x0001);
    // Serial.println("Initialization completed");
    // byte message[] = {0xCC, 0x00, 0x44, 0x07, 0x00 , 0xDD, 0xF4, 0x01};

    // rotary_valve.write(message, sizeof(message));
    // Serial.println(ret);
}
String response;
byte a; 

void loop() {

    while (1)
    {   
        // Serial.print("ack");
        // a = rotary_valve.read();
        ret = rotary_valve.GetResponseRX();
        if (ret == -1) {
            delay(50);
            continue;
        }
        Serial.println(ret);
        // response = rotary_valve.PrintResponse();
        delay(100);
        Serial.println(response);
        if (ret != -1) {return;}
    }



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