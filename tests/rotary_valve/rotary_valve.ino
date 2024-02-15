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

#define ROT_VALVE_RX   10
#define ROT_VALVE_TX   9

SoftwareSerial rotary_valve(ROT_VALVE_RX, ROT_VALVE_TX, true);

void setup() {
    Serial.begin(9600);
    rotary_valve.begin(9600);
    // pinMode(ROT_VALVE_RX, OUTPUT);
    // pinMode(ROT_VALVE_TX,INPUT);
}

void loop() {
    byte message[] = {0xCC, 0x00, 0x44, 0x07, 0x00 , 0xDD, 0xF4, 0x02};

    rotary_valve.write(message, sizeof(message));
    
    // rotary_valve.end();
    while (1)
    {
        // do nothing
    }
}