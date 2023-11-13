/**
 * 
*/

#include "Arduino.h"
#include "Comm.h"

/// @brief read bytes from serial to received buffer
/// @return true/1 if success
bool CommGetBytes(comm_buffer_t* comm_input_buffer)
{
    while (Serial.available() && comm_input_buffer->size < BUFFER_SIZE)
    {
        comm_input_buffer->data[comm_input_buffer->size++] = Serial.read();
    }
    if (comm_input_buffer->size > 0)
    {
        return true;
    }
    return false;
}


/// @brief write bytes from transmit buffer to serial
/// @return true/1 if success 
bool CommSendBytes(comm_buffer_t* comm_output_buffer)
{
    if (comm_output_buffer->size == 0)
    {
        return false;
    }
    Serial.write(comm_output_buffer->data,comm_output_buffer->size);
    memset(comm_output_buffer->data, 0, comm_output_buffer->size);
    comm_output_buffer->size = 0;
    return true;
}
