# Communication protocol #

## Idea ##
- In order to control multiple peripheral devices from one controller, it is necessary to have a communication protocol in place to process command accurately.

## Process ##

        +-------------+  ----Command--->  +-----------+
        |  Controller |                   |   Device  |
        +-------------+  <---Response---  +-----------+

        Successful response sequence: Ack -> Data
        Unsuccessful response sequence: Nack 

## Frame Structure ##
- Every command from the central controller has the following structure for every frame:

        <start_sequence> <component_ID> <command_ID> <num_bytes>   <input>     <CRC>   <stop_sequence>
        <---2 Bytes----> <--1 Bytes---> <-1 Bytes--> <-1 Bytes-> <num_bytes> <1 Bytes> <---2 Bytes--->

        Total: 8 + num_bytes (num_bytes is the length of input in bytes (each input is 4 bytes (32 bits)). Ex: a command with no input is 8 bytes)
- Every response from the peripheral devices to the central controller have the following structure:

        <start_sequence> <component_ID> <response_ID> <num_bytes>  <content>     <CRC> <stop_sequence>
        <---2 Bytes----> <--1 Bytes---> <--1 Bytes--> <-1 Bytes-> <num_bytes> <1 Bytes><---2 Bytes--->
        Total: 8 + num_bytes (num_bytes > 0, each content data is 4 byte (32 bits))

## Start/Stop sequence/bits

- Command start sequence: `ADDA`
- Response start sequence: `BEEB`
- Stop  sequence: `FDDF` 

## Component ID ##

- Convention: Component ID should start with ABCEDF (max: 96 components)

    | Component         |    ID    | 
    | :---------------- | :------: | 
    | Masterflex pump 1 |    B1    | 
    | Masterflex pump 2 |    B2    |
    | Pump 1 Arduino    |    A1    |
    | Pump 2 Arduino    |    A2    |

## Command ID ##

- Convention: command ID is in HEX and should contain with number (start with 1-9) (max: 90 commands/component)

    | Component         | Command     | Command ID | Parameters | Parameter length (Bytes) |
    | :---------------- | :--------:  | :--------: | :--------: | :----------------------: |
    | Masterflex pump   |  Run pump   |     0x11     |    None    |         0                |
    | Masterflex pump   |  Stop pump  |     0x12     |    None    |         0                |
    | Masterflex pump   |  Dispense   |     0x13     |    amount (uL)  |    2 (max: 65 ml ) |
    | Masterflex pump   |  Reverse/Change direction    |   0x14    |    None |     0        |
    | Masterflex pump   |  Set direction    |   0x15    |   CW/CCW |      1        |
    | Masterflex pump   |  Run for a duration    |   0x16    |    time (ms) |     2 (max: 65s)        |
    | Masterflex pump   |  Set max voltage    |   0x17    |    voltage (mV) |     2 (max: 65V)       |
    | Masterflex pump   |  Set min voltage    |   0x18    |    voltage (mV) |     2 (max: 65V)       |
    | Masterflex pump   |  Set max flow rate  |   0x19    |    ul/min       |     2 (max: 65 ml/min)       |
    | Masterflex pump   |  Set min flow rate  |   0x20    |    ul/min       |     2 (max: 65 ml/min)       |
    | Masterflex pump   |  Set speed  |   0x21    |    ul/min       |     2 (max: 65 ml/min)       |
    | Masterflex pump   |  Start priming  |   0x22    |    None       |     0       |
    | Masterflex pump   |  Stop priming  |   0x23    |    None       |     0       |
    | Masterflex pump   |  Prime for a duration  |   0x24    |    time (ms) |     2 (max: 65s)        |


## Response ID ##


- Convention: response ID is in HEX and should start with number (1-9) and finish with letter (A-F) (max: 54 commands/component)

    | Reponse Type           |   Meaning        | Reponse ID     | Parameters | Parameter length (Bytes) |
    | :--------------------- | :-------------:  |  :-------------:  | :--------: | :----------------------: |
    | Acknowledge (Ack)            |    Received command              | 0x1A       |     <CPNT_ID> <CMD_ID>   |         4                |
    | Not acknowledge (Nack) |  Not received correct command | 0x9F |     <CPNT_ID> <CMD_ID>   |         4                |
    | Data |  Returned value for a command | 0x2D |     returned value   |         depend on command (typ. 2 bytes)                |

