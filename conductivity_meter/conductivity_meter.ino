/**
 * @brief 
 * 
 */
#include <SoftwareSerial.h>


char buffer;
bool buffer_empty = true;
#define METER_TX         8
#define METER_RX         9
// SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
SoftwareSerial cond_meter(METER_TX, METER_RX, false);
void setup() {
    Serial.begin(9600);
    cond_meter.begin(9600);
    Serial.println("serial limit test 1.0"); // so I can keep track of what is loaded
    // cond_meter_transmitter.write(0x11);
    cond_meter.write("GETMEAS\r");
    
    // delay(10000);
    // cond_meter.write(0x11);
}

void loop() {
  //expect a string like wer,qwe rty,123 456,hyre kjhg,
  //or like hello world,who are you?,bye!,
    while (cond_meter.available())  {
        buffer = cond_meter.read();
        buffer_empty = false;
        Serial.print(buffer, HEX);
    }
    if (!buffer_empty){
        Serial.println();
        buffer_empty = true;
    }
    delay(10);
}