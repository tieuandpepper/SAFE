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
/*----------------------------------------------------------------------------------------------------------*/
// Define pinout
#define SPEED_CONTROL       11
#define SPEED_FEEDBACK      A0 // analog pin
#define REMOTE_CONTROL      10
#define CLOCKWISE_CONTROL   9
#define PRIME_CONTROL       8

// Define state
#define STATE_ON LOW
#define STATE_OFF HIGH
#define CW HIGH
#define CCW LOW
#define PRIME_ON LOW
#define PRIME_OFF HIGH

void PumpControl(uint8_t state);
void PumpDirection(uint8_t input_direction);
void PumpPrime(uint8_t prime);
void GetSpeed(void);

/*----------------------------------------------------------------------------------------------------------*/
float curr_real_speed = 0;
float curr_setting_speed = 0;
uint8_t operate_state = STATE_OFF;
uint8_t direction_state = CW;
uint8_t prime_state = PRIME_OFF;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SPEED_CONTROL, OUTPUT);
  pinMode(REMOTE_CONTROL, OUTPUT);
  pinMode(CLOCKWISE_CONTROL, OUTPUT);
  pinMode(PRIME_CONTROL, OUTPUT);
  PumpControl(STATE_OFF);
  PumpDirection(CW);
  PumpPrime(PRIME_OFF);
}
/*----------------------------------------------------------------------------------------------------------*/
int x = 1;
void loop() {
  // put your main code here, to run repeatedly:
  
}
/*----------------------------------------------------------------------------------------------------------*/
void PumpControl(uint8_t state)
{
  operate_state = state;
  digitalWrite(REMOTE_CONTROL, operate_state);
}
/*----------------------------------------------------------------------------------------------------------*/
void PumpDirection(uint8_t input_direction)
{
  direction_state = input_direction;
  digitalWrite(CLOCKWISE_CONTROL, direction_state);
}
/*----------------------------------------------------------------------------------------------------------*/
void PumpPrime(uint8_t prime)
{
  prime_state = prime;
  digitalWrite(PRIME_CONTROL, prime_state);
}
/*----------------------------------------------------------------------------------------------------------*/
void PumpGetSpeed(void)
{
  curr_real_speed = analogRead(SPEED_FEEDBACK)*(100/1023.0);
  Serial.println(curr_real_speed);
}
/*----------------------------------------------------------------------------------------------------------*/
void PumpSetSpeed(float speed_percent)
{
  curr_setting_speed = speed_percent * 255 / 100;
  analogWrite(SPEED_CONTROL,curr_setting_speed);
  Serial.println(curr_setting_speed);
}
