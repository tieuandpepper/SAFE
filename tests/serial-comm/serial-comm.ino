// void setup()
// {
//   Serial.begin(9600);
//   pinMode(8,INPUT);
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// byte buffer[64];
// int idx=0;
// bool received = false;

// void loop()
// {
//   if (Serial.available())
//   {
//     idx = Serial.readBytes(buffer, 64);
//     received = true;
//     Serial.print("Data received: ");
//     digitalWrite(LED_BUILTIN, HIGH);
//   }
//   if (received && digitalRead(8)==HIGH)
//   {
//     digitalWrite(LED_BUILTIN, LOW);
//     Serial.write(buffer,idx);
//     idx = 0;
//     received = false;
//   }
// }

int x;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  Serial.print(x + 1);
}