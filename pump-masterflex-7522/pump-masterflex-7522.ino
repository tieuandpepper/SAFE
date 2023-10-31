/*
 * PUMP CONTROL MASTERFLEX 7522
 * Arduino UNO to DB25
 * DB25 pinout:
 * DB25_P1 : Speed control INPUT voltage 0-10V
 * DB25_P3 : Speed control INPUT GROUND 
 * DB25_P5 : Speed signal OUTPUT GROUND
 * DB25_P14: Speed signal OUTPUT voltage 0-10V
 * DB25_P15: Remote START/STOP INPUT
 * DB25_P16: Remote CW/CCW INPUT
 * DB25_P17: Remote START/STOP, CW/CCW, Prime GROUND
 * DB25_P20: Remote prime INPUT
 *                 +-----------------------+
 *                 |    Wiring diagram     |
 *                 |          DB25         |
 *                 +-----------------------+
 *   _____________________________________________________
 *   | 13  12  11  10  09  08  07  06  05  04  03  02  01 |
 *    \  25  24  23  22  21  20  19  18  17  16  15  14  / 
 *     \________________________________________________/
 * 
 *                 +-----------------------+  
 *                 |    Wiring diagram     |
 *                 |      Arduino UNO      |
 *                 +-----------------------+ 
 *                     _________________
 *                     |           SCL-|-
 *                     |           SDA-|-
 *                     |          AREF-|-
 *                     |           GND-|- DB25_P3
 *                     |           D13-|- 
 *                    -|-IOREF     D12-|- 
 *                    -|-RESET     D11-|- DB25_P1 
 *                    -|-3.3V      D10-|- DB25_P15
 *                    -|-5V        D09-|- DB25_P16
 *           DB25_P17 -|-GND       D08-|- DB25_P20
 *            DB25_P5 -|-GND           | 
 *                    -|-VIN       D07-|-
 *                     |           D06-|-
 *           DB25_P14 -|-A0        D05-|-
 *                    -|-A1        D04-|-
 *                    -|-V2        D03-|-
 *                    -|-A3        D02-|-
 *                    -|-A4         TX-|-
 *                    -|-A5         RX-|-
 *                     |_______________|
 * 
 */
// Define pinout
#define SPEED_CONTROL       11
#define REMOTE_CONTROL      10
#define CLOCKWISE_CONTROL   9
#define PRIME_CONTROL       8
#define SPEED_FEEDBACK      A0 // analog pin
#define SPEED_CONTROL_VERIFY A1 //analog pin

// Define state
#define PUMP_ON   LOW
#define PUMP_OFF  HIGH
#define CW        HIGH
#define CCW       LOW
#define PRIME_ON  LOW
#define PRIME_OFF HIGH

// define a pump structure
typedef struct PumpStruct {
  double speed_actual = 0;
  double speed_setting = 0;
  uint8_t state_operate = PUMP_OFF;
  uint8_t state_direction = CW;
  uint8_t state_prime = PRIME_OFF;
} PumpData;

// define functions
void PumpStart(PumpData);
void PumpStop(PumpData);
void PumpDirection(PumpData, uint8_t);
void PumpPrime(PumpData, uint8_t);
void PumpGetSpeed(PumpData);
void PumpSetSpeed(PumpData, double);

/*----------------------------------------------------------------------------------------------------------*/
PumpData pump;
char buffer[100];

/// @brief abc
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SPEED_CONTROL, OUTPUT);
  pinMode(REMOTE_CONTROL, OUTPUT);
  pinMode(CLOCKWISE_CONTROL, OUTPUT);
  pinMode(PRIME_CONTROL, OUTPUT);
  pinMode(SPEED_CONTROL_VERIFY,INPUT_PULLUP);
  pinMode(SPEED_FEEDBACK, INPUT_PULLUP);
  PumpDirection(pump,pump.state_direction);
  PumpPrime(pump, pump.state_prime);
  PumpStart(pump);
}

/// @brief something
void loop() {
  // put your main code here, to run repeatedly:
  PumpSetSpeed(pump,50);
  PumpGetSpeed(pump);
  delay(1000);
}



/// @brief start the pump by driving the input voltage pin to LOW
void PumpStart(PumpData p)
{
  if (p.state_operate == PUMP_OFF)
  {
    p.state_operate = PUMP_ON;
    digitalWrite(REMOTE_CONTROL, p.state_operate);
  }
}

/// @brief STOP the pump by driving the input voltage pin to HIGH
void PumpStop(PumpData p)
{
  if (p.state_operate == PUMP_ON)
  {
    p.state_operate = PUMP_OFF;
    digitalWrite(REMOTE_CONTROL, p.state_operate);
  }
}

/// @brief 
/// @param input_direction 
void PumpDirection(PumpData p, uint8_t input_direction)
{
  p.state_direction = input_direction;
  digitalWrite(CLOCKWISE_CONTROL, p.state_direction);
}

/// @brief 
/// @param prime 
void PumpPrime(PumpData p, uint8_t prime)
{
  p.state_prime = prime;
  digitalWrite(PRIME_CONTROL, p.state_prime);
}

/// @brief 
/// @param  
void PumpGetSpeed(PumpData p)
{
  int output = analogRead(SPEED_FEEDBACK);
  double voltage = ((double)output*5/1023);
  p.speed_actual = voltage*100/5;
  Serial.print("Output [pwm/1023] =");
  Serial.print(output);
  Serial.print(", voltage [V/5] = ");
  Serial.print(voltage);
  Serial.print(", speed % = ");
  Serial.println(p.speed_actual);
}

/// @brief 
/// @param speed_percent 
void PumpSetSpeed(PumpData p,  double speed_percent)
{
  p.speed_setting = speed_percent;
  analogWrite(SPEED_CONTROL,(int)p.speed_setting *255/100);
  Serial.print("SetSpeed % = ");
  Serial.println(p.speed_setting);
  double pwm = (double)analogRead(SPEED_CONTROL_VERIFY)*5/1023;
  Serial.print("VerifyVoltage [V/5] = ");
  Serial.println(pwm);
}
