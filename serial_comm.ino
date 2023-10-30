/*
 * COMMUNICATION PROTOCOL
 * Command:  <ABBA> <COMPONENT> <COMMAND> <CRC> <BAAB>
 *            <2B>     <2B>       <2B>     <2B>  <2B> ==> 10 Bytes
 * Response: <FEEF> <COMPONENT> <COMMAND> <RESPONSE> <CRC> <EFFE>
 * 
 * .0
 * PUMP:
 *   - Start: 
*/
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // nothing
}