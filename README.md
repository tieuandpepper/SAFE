# SAFE
SAFE autonomous lab
# 1. Overall Diagram
![SAFE-Diagram](https://github.com/user-attachments/assets/fae1f939-2ada-41d3-9028-8466080ab108)
Primary Arduino: main microcontroller, controlling most of the equipment, including the secondary Arduino.

Secondary Arduino: controlling rotary valve (the conflict between UART and I2C communication protocols using 1 Arduino -> Use 1 for UART (rotary valve) and 1 for I2C (the primary Arduino).

System control computer: connect to primary Arduino and Conductivity Meter

# 2. Arduino Connections
The header file arduino_pinout.h contains all pin connections for every piece of equipment for 2 Arduino modules. Change the values of macros in this file when the pin connections are changed. Other files import this file (use #include at the beginning of a header/c file).
<img width="500" alt="image" src="https://github.com/user-attachments/assets/09841110-2cb7-4bbd-9491-7ccd51f3b5e3">

In addition, the drawing and table in this file show other connections that are not required to be specified in macros to use explicitly (power supply and ground connection).

<img width="500" alt="image" src="https://github.com/user-attachments/assets/19a4e7ad-a8c7-4917-9dcf-b6aca13bc7d9">
<img width="500" alt="image" src="https://github.com/user-attachments/assets/ba1b5fa7-6c3b-40a0-9155-f664d3f2bb2f">

For devices using I2C, their clock (SCL) and data (SDA) lines are connected to SCL and SDA pins on Arduino. The picture below shows an example of an I2C communication setup. The addresses of all I2C devices used are specified in arduino_setting.h. Change the values of macros in this file when the pin connections are changed. Other files import this file (use #include at the beginning of a header/c file).

![image](https://github.com/user-attachments/assets/409806a7-0005-41dd-a16b-761bc0d3f5cf)

# 3. Arduino program
The main Arduino script is arduino_main.ino. This script will import specs from arduino_pinout.h and arduino_setting.h to set up connections. To control equipment, it uses 2 modules:
  - serial_comm: serial communication between computer and Arduino. This module is used to get and translate commands, assemble the responses, and send them back.
  - device_controller: this is where a command is processed. There are 2 layers in this module:
    - First layer: device. This layer is used in arduino_main.ino, used to get the device controller that the command is targeted.
    - Second layer: instruction. This layer is described in device_controller.h and device_controller.c. It filters instructions and send them as well as their operands (if needed) to the device driver. The device drivers are located in src/device.
<img width="529" alt="image" src="https://github.com/user-attachments/assets/e735682c-86fa-427c-b617-d8d39fd366c4">

# 4. Communication structure
The communication protocol has 2 types of structure: command and response. They are described in serial_comm module.
  - Command: <TARGET;INSTRUCTION;OPERAND>
    - TARGET: intended device/instrument
    - INSTRUCTION: the command (move, dispense, measure, etc)
    - OPERAND: additional data required (dispense amount, delay amount). This part uses integer only, so multiply everything by 1000. For example, <PUMP;DISPENSE;2000> means dispense 2ml.
  - Response: <RESP;SOURCE;RESPONSE_TYPE;ERROR_CODE;DATA>
    - SOURCE: device that sends the response
    - RESPONSE_TYPE: the type of response: valid (valid command), invalid (failed command execution), feedback (send back data), unsupported (no support for the corresponding command)
    - ERROR_CODE: what error has happened
    - DATA: corresponding data, if RESPONSE_TYPE is feedback

The description of command targets and instructions can be found in command_list.h. The target code is also for response sources.

The description of response type is located in comm_serial.h

The description of response error code is located in device/response_error_code.h

<img width="420" alt="image" src="https://github.com/user-attachments/assets/b7172996-a88e-4af0-9ba8-c04b1869184e">


# 5. ARDUINO_CENTRAL and ARDUINO_VERBOSE
ARDUINO_CENTRAL is created so that both primary and secondary Arduino control programs can be reused in 1 file (arduino_main.ino) since they require several layers of dependencies and drivers.

  - ARDUINO_CENTRAL=1: used for primary Arduino

  - ARDUINO_CENTRAL=0: used for secondary Arduino

ARDUINO_VERBOSE is used to control how talkative an Arduino is. Talkative Arduino will send back more data than necessary for debugging purposes (ARDUINO_VERBOSE=1).

# 6. Conductivity Meter
The driver for the conductivity meter is located in python-controller/CondMeterA215.py. It connects directly to computer.

# 7. Python controller
It is located at python-controller/python_controller.py. It is used to automatically assemble a list of commands and send them sequentially to the primary Arduino. It only needs the input of additive percentages, separated by commas.

For manual control, use python-controlller/manual_comm.py. Define the command list and it will send command sequentially to the Arduino.
