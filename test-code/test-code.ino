void setup()
{
    Serial.begin(9600);
}

int percent = 0;
void loop()
{
    percent = analogRead(A0);
    Serial.println(percent);
    delay(100);
}