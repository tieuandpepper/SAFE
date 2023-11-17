# SYSTEM CONTROLLER
import serial
import time
# from CondMeterA215 import *

# comm port used
ARDUINO_PORT = "/dev/tty.usbserial-1420"
# COND_METER_PORT = "COM6"
baurd_rate = 9600

def PrintResponse(comm_port):
  response = "No response"
  while response.find("RESP") == -1:
    time.sleep(0.2)
    response = comm_port.readline().decode('utf-8')
  
  print("-> ", response)

def PrintCommand(comm_port):
  command = "Invalid command"
  while command.find("Target") == -1:
    time.sleep(0.2)
    command = comm_port.readline().decode('utf-8')
  
  print("<- ", command)

def SendCommand(comm_port, command):
  comm_port.write(bytes(command,'utf-8'))

def main():
  arduino = serial.Serial(port = ARDUINO_PORT,  baudrate=baurd_rate, timeout=.5)
  # cond_meter = ConductivityMeter(COND_METER_PORT, baurd_rate)

  ready_msg = ""
  while ready_msg.find("READY") == -1:
    time.sleep(0.2)
    ready_msg = arduino.readline().decode('utf-8')
    if len(ready_msg) > 0:
      print(ready_msg)
  
  command_list = ["pump2,disp,5000.",
                  "mixer, run, 10000.",
                  "pump1, disp, 3000.",
                  ]
  for cmd in command_list:
    SendCommand(arduino, cmd)
    PrintCommand(arduino)
    PrintResponse(arduino)

  # Measure the conductivity
  # cond_meter.sendCommand("GETMEAS",[])

  arduino.close()
  




if __name__ == '__main__':
    main()