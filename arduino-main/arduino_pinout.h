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
 *                         |           SCL-|- temp sensor SCL
 *                         |           SDA-|- temp sensor SDA
 *                         |          AREF-|-
 *                         |           GND-|- 
 *                         |           D13-|- 
 *                        -|-IOREF     D12-|- 
 *                        -|-RESET     D11-|- 
 *                        -|-3.3V      D10-|- rotary valve TX
 *        mixer_relay_Vcc -|-5V        D09-|- rotary valve RX
 *                        -|-GND       D08-|- mixer relay enable
 *                        -|-GND           | 
 *                        -|-VIN       D07-|- lighter enable pin
 *                         |           D06-|- lighter initialization pin
 *                        -|-A0        D05-|- mixing_pump DB25_P01
 *                        -|-A1        D04-|- mixing_pump DB25_P15
 *                        -|-A2        D03-|- mixing_pump DB25_P16
 *   mixing_pump DB25_P14 -|-A3        D02-|- mixing_pump DB25_P20
 *      transfer_pump SDA -|-A4         TX-|-
 *      transfer_pump SCL -|-A5         RX-|-
 *                         |_______________|
 * 
 * 
 * 
 *                     +-----------------------+  
 *                     |    Wiring diagram     |
 *                     |        Lighter        |
 *                     +-----------------------+ 
 *                       __________________________________________
 *                       |   ______________         __________    |
 *                       |   |            |         |  pMOS  |    |
 *                       |   |            |         | Source-|----+    
 *                       +---|---button---|---------|-Drain  |     
 *                           |            |         |  Gate  |
 *                           |            |         |____|___|
 *                           |            |              |
 *         Arduino GND ------| - initial +|----+     Arduino D07
 *                           |   ization  |    |
 *                           |            |  Arduino D06
 *                           |  VCC   GND |
 *                           |___|_____|__|
 *                               |     |
 *                            5V Power supply
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
 *      LIGHTER          |    ENABLE - ON/OFF BUTTON   |    D07      |               
 *      LIGHTER          |         INITIALIZATION      |    D06      |
 *      LIGHTER          |       INITIALIZATION_GND    |    GND      |      
 *      LIGHTER          |          POWER_VCC          |             |      3.7-5 V
 *      LIGHTER          |          POWER_GND          |             |       GND
 *      TEMP SENSOR      |             VCC             |     5V      |
 *      TEMP SENSOR      |             GND             |     GND     |
 *      TEMP SENSOR      |          SCL Clock          |     SCL     |
 *      TEMP SENSOR      |          SDA Data           |     SDA     |
 *      ROTARY VALVE     |           RS232 RX          |     D09     |
 *      ROTARY VALVE     |           RS232 TX          |     D10     |
 *      ROTARY VALVE     |           RS232 GND         |     GND     |
 *      ROTARY VALVE     |           DC 24V +          |             |       24V
 *      ROTARY VALVE     |           DC 24V -          |             |       GND
 *      TRANSFER PUMP    |             VCC             |     5V      |
 *      TRANSFER PUMP    |             GND             |     GND     |
 *      TRANSFER PUMP    |             SDA             |     A4      |
 *      TRANSFER PUMP    |             SCL             |     A5      |
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
#define PIN_MIXING_PUMP_PRIME_CONTROL       2
#define PIN_MIXING_PUMP_SPEED_FEEDBACK      A3 // analog pin

// Mixer pin
#define PIN_MIXER_ENABLE                    8

// Lighter pins
#define PIN_LIGHTER_INIT                    6
#define PIN_LIGHTER_ENABLE                  7

// rotary valve
#define PIN_ROTARY_VALVE_TX                 10
#define PIN_ROTARY_VALVE_RX                 9

// I2C Address
#define I2C_ADDR_TEMP_SENSOR                0x10
#define I2C_ADDR_TRANSFER_PUMP              0x67