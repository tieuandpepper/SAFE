void setup()
{
    Serial.begin(9600);
    pinMode(12,INPUT);
}

int percent = 0;
void loop()
{
    percent = analogRead(A0);
    Serial.println(percent);
    percent = digitalRead(12);
    Serial.println(percent);
    delay(100);
}