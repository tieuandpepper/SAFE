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
 *  
 *                     +-----------------------+
 *                     |    Wiring diagram     |
 *                     |          DB25         |
 *                     +-----------------------+
 *      _____________________________________________________
 *      [ 13  12  11  10  09  08  07  06  05  04  03  02  01 ]
 *      [                                                    ]
 *       \  25  24  23  22  21  20  19  18  17  16  15  14  / 
 *        \________________________________________________/
 *  
 *                     +-----------------------+  
 *                     |    Wiring diagram     |
 *                     |      Arduino UNO      |
 *                     +-----------------------+ 
 *                         _________________
 *                         |           SCL-|-
 *                         |           SDA-|-
 *                         |          AREF-|-
 *                         |           GND-|- 
 *                         |           D13-|- 
 *                        -|-IOREF     D12-|- 
 *                        -|-RESET     D11-|- 
 *                        -|-3.3V      D10-|- 
 *        mixer_relay_Vcc -|-5V        D09-|- 
 *                        -|-GND       D08-|- mixer_relay_in
 *                        -|-GND           | 
 *                        -|-VIN       D07-|- 
 *                         |           D06-|- 
 *                        -|-A0        D05-|- DB25_P01
 *                        -|-A1        D04-|- DB25_P15
 *                        -|-A2        D03-|- DB25_P16
 *               DB25_P14 -|-A3        D02-|- DB25_P20
 *                        -|-A4         TX-|-
 *                        -|-A5         RX-|-
 *                         |_______________|
 * 
 * 
 *                     +-----------------------+
 *                     | PINOUT & POWER SUPPLY |
 *                     |       & GROUND        |
 *                     +-----------------------+
 *                 
 *      COMPONENT        |             PIN             | ARDUINO PIN | POWER SUPPLY 
 *      -----------------+-----------------------------+-------------+--------------
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-01    |    D05      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-03    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-05    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-14    |    A3       |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-15    |    D04      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-16    |    D03      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-17    |    GND      |              
 *      MIXING PUMP      |    CONNECTOR-DB25 PIN-20    |    D02      |              
 *      MIXER            |          RELAY - GND        |    GND      |              
 *      MIXER            |          RELAY - VCC        |     5V      |          
 *      MIXER            |          RELAY - IN         |    D08      |              
 *  
 */

// Masterflex mixing pump pinout
#define PIN_MIXING_PUMP_SPEED_CONTROL       5
#define PIN_MIXING_PUMP_REMOTE_CONTROL      4
#define PIN_MIXING_PUMP_CLOCKWISE_CONTROL   3
#define PIN_MIXING_PUMP_PRIME_CONTROL       2
#define PIN_MIXING_PUMP_SPEED_FEEDBACK      A3 // analog pin

// Mixer pin
#define PIN_MIXER_ENABLE                    8