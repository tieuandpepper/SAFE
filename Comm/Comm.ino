#include "Comm.h"

comm_buffer_t comm_input_buffer;
comm_buffer_t comm_output_buffer;
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
    if (CommGetBytes(&comm_input_buffer))
    {
        for (int i = 0; i < comm_input_buffer.size; ++i)
        {
          Serial.write(comm_input_buffer.data[i]);
          Serial.print(" or ");
          Serial.println(int(comm_input_buffer.data[i]));        
        }
        Serial.println("Data received");
    }
    if (comm_input_buffer.size > 0 && digitalRead(8)==HIGH)
    {
        for (int i = 0; i < comm_input_buffer.size; ++i)
        {
          Serial.write(comm_input_buffer.data[i]);
          Serial.print(" or ");
          Serial.println(int(comm_input_buffer.data[i]));        
        }
        memset(comm_input_buffer.data,0, comm_input_buffer.size);
        comm_input_buffer.size = 0;
        Serial.println("Data sent");
    }
    delay(5000);
}
