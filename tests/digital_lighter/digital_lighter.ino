#include <Arduino.h>
#define CHARGER_PIN 7
#define ENABLE_PIN  6
void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  pinMode(CHARGER_PIN, OUTPUT);
  pinMode(ENABLE_PIN,OUTPUT);
  digitalWrite(CHARGER_PIN,HIGH);
  delay(5000);
  digitalWrite(CHARGER_PIN,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ENABLE_PIN, HIGH);
  Serial.println("turn ON lighter");
  delay(2000);
  digitalWrite(ENABLE_PIN, LOW);
  Serial.println("turn OFF lighter");
  delay(4000);
  // digitalWrite(CHARGER_PIN,HIGH);
  // while(1) {};
}
