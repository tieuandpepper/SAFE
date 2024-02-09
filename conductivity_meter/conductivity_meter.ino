/**
 * @brief 
 * 
 */
#include <SoftwareSerial.h>


String buffer;
#define METER_TX         12
#define METER_RX         13
SoftwareSerial cond_meter(METER_TX, METER_RX);
void setup() {
    Serial.begin(9600);
    cond_meter.begin(9600);
    Serial.println("serial delimit test 1.0"); // so I can keep track of what is loaded
    cond_meter.write("GETLOG\r");
}

void loop() {

  //expect a string like wer,qwe rty,123 456,hyre kjhg,
  //or like hello world,who are you?,bye!,
    if (cond_meter.available())  {
        buffer = cond_meter.read();
        // Serial.print(buffer); //prints string to serial port out
        Serial.print(buffer);
    }
    delay(1);
}