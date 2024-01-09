void setup()
{
    Serial.begin(9600);
}

float voltage = 0;
void loop()
{
    voltage = 3.3 * analogRead(A0) / 1024;
    Serial.println(voltage);
    delay(100);
}