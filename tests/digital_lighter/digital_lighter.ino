
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  delay(60000);
  digitalWrite(8,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  Serial.println("turn ON lighter");
  delay(5000);
  digitalWrite(13, LOW);
  Serial.println("turn OFF lighter");
  delay(10000);
  // while(1) {};
}
