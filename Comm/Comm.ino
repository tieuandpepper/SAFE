#include "Comm.h"

msg_t buffer_input;
msg_t buffer_output;
uint32_t count = 0;
void setup()
{
    Serial.begin(9600);
    pinMode(8, INPUT);
}

void loop()
{
    count++;
    Serial.println(count);
    if (CommGetBytes(&buffer_input))
    {
        for (int i = 0; i < buffer_input.size; ++i)
        {
          Serial.write(buffer_input.data[i]);
          Serial.print(" or ");
          Serial.println(int(buffer_input.data[i]));        
        }
        Serial.println("Data received");
    }
    if (buffer_input.size > 0 && digitalRead(8)==HIGH)
    {
        for (int i = 0; i < buffer_input.size; ++i)
        {
          Serial.write(buffer_input.data[i]);
          Serial.print(" or ");
          Serial.println(int(buffer_input.data[i]));        
        }
        memset(buffer_input.data,0, buffer_input.size);
        buffer_input.size = 0;
        Serial.println("Data sent");
    }
    delay(5000);
}
