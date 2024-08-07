/*
 * PUMP CONTROL MASTERFLEX 7522
 * Arduino UNO to DB25
 * DB25 pinout:
 * DB25_P1 : Speed control INPUT voltage 0-10V (YELLOW)
 * DB25_P3 : Speed control INPUT GROUND (RED)
 * DB25_P5 : Speed signal OUTPUT GROUND (ORANGE)
 * DB25_P14: Speed signal OUTPUT voltage 0-10V (GREEN)
 * DB25_P15: Remote START/STOP INPUT (PURPLE)
 * DB25_P16: Remote CW/CCW INPUT (BLUE)
 * DB25_P17: Remote START/STOP, CW/CCW, Prime GROUND (GRAY)
 * DB25_P20: Remote prime INPUT (BROWN)
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
 *                         |           SCL-|- temp sensor SCL
 *                         |           SDA-|- temp sensor SDA
 *                         |          AREF-|-
 *                         |           GND-|- 
 *                         |           D13-|- 
 *                        -|-IOREF     D12-|- 
 *                        -|-RESET     D11-|- 
 *                        -|-3.3V      D10-|- mixer relay enable
 *        mixer_relay_Vcc -|-5V        D09-|- rotary valve TX
 *                        -|-GND       D08-|- rotary valve RX
 *                        -|-GND           | 
 *                        -|-VIN       D07-|- lighter charger enable
 *                         |           D06-|- lighter on/off enable
 *                        -|-A0        D05-|- mixing_pump DB25_P01
 *                        -|-A1        D04-|- mixing_pump DB25_P15
 *                        -|-A2        D03-|- mixing_pump DB25_P16
 *   mixing_pump DB25_P14 -|-A3        D02-|- 
 *      transfer_pump SDA -|-A4         TX-|-
 *      transfer_pump SCL -|-A5         RX-|-
 *                         |_______________|
 * 
 * 
 * 
 *                     +-----------------------+
 *                     | PINOUT & POWER SUPPLY |
 *                     |       & GROUND        |
 *                     +-----------------------+
 *                 
 *      COMPONENT        |             PIN             | ARDUINO PIN | POWER SUPPLY 
 *      -----------------+-----------------------------+-------------+--------------
 *      MIXING PUMP      |    DB25 PIN-01 YELLOW       |    D05      |              
 *      MIXING PUMP      |    DB25 PIN-03 RED          |    GND      |              
 *      MIXING PUMP      |    DB25 PIN-05 ORANGE       |    GND      |              
 *      MIXING PUMP      |    DB25 PIN-14 GREEN        |    A3       |              
 *      MIXING PUMP      |    DB25 PIN-15 PURPLE       |    D04      |              
 *      MIXING PUMP      |    DB25 PIN-16 BLUE         |    D03      |              
 *      MIXING PUMP      |    DB25 PIN-17 GRAY         |    GND      |              
 *      MIXER            |          RELAY - GND        |    GND      |              
 *      MIXER            |          RELAY - VCC        |     5V      |          
 *      MIXER            |          RELAY - IN         |    D10      |              
 *      LIGHTER          |          ENABLE             |    D06      |    
 *      LIGHTER          |          CHARGER            |    D07      |    
 *      LIGHTER          |          POWER_VCC          |             |      9-12V => 2.5->3A
 *      LIGHTER          |          POWER_GND          |             |       GND
 *      TEMP SENSOR      |             VCC             |     5V      |
 *      TEMP SENSOR      |             GND             |     GND     |
 *      TEMP SENSOR      |          SCL Clock          |     SCL     |
 *      TEMP SENSOR      |          SDA Data           |     SDA     |
 *      ROTARY VALVE     |           RS232 RX          |     D08     |
 *      ROTARY VALVE     |           RS232 TX          |     D09     |
 *      ROTARY VALVE     |           RS232 GND         |     GND     |
 *      ROTARY VALVE     |           DC 24V +          |             |       24V
 *      ROTARY VALVE     |           DC 24V -          |             |       GND
 *      TRANSFER PUMP    |             VCC             |     5V      |
 *      TRANSFER PUMP    |             GND             |     GND     |      
 *      TRANSFER PUMP    |             SDA             |     A4      |      4.7kOhm PUR
 *      TRANSFER PUMP    |             SCL             |     A5      |      4.7kOhm PUR
 *      
 * 
 *      
 *
 *  
 */

// Masterflex mixing pump pinout
#define PIN_MIXING_PUMP_SPEED_CONTROL       5
#define PIN_MIXING_PUMP_REMOTE_CONTROL      4
#define PIN_MIXING_PUMP_CLOCKWISE_CONTROL   3
#define PIN_MIXING_PUMP_SPEED_FEEDBACK      A3 // analog pin

// Mixer pin
#define PIN_MIXER_ENABLE                    10

// Lighter pins
#define PIN_LIGHTER_ENABLE                  6
#define PIN_LIGHTER_CHARGER                 7

// rotary valve
#define PIN_ROTARY_VALVE_RX                 8
#define PIN_ROTARY_VALVE_TX                 9