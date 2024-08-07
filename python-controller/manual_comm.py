# SYSTEM CONTROLLER
import serial
import time
import CondMeterA215 as cm

def PrintResponse(comm_port):
  response = "No response"
  while response.find("RESP") == -1:
    time.sleep(0.2)
    response = comm_port.readline().decode('utf-8') 
  
  print("-> ", response)

def PrintCommand(comm_port):
  command = "Invalid command"
  while command.find("CMD") == -1:
    time.sleep(0.2)
    command = comm_port.readline().decode('utf-8')
  
  print("<- ", command)

def SendCommand(comm_port, command):
  comm_port.write(bytes(command,'utf-8'))


###################################################################################
def main():
  # comm port used
  ARDUINO_PORT = "/dev/tty.usbserial-1110"
  # ARDUINO_PORT = "COM9"
  # COND_METER_PORT = "COM15"
  baud_rate = 115200
  arduino = serial.Serial(port = ARDUINO_PORT,  baudrate=baud_rate, timeout=.5)
  # cond_meter = cm.ConductivityMeter(COND_METER_PORT, baud_rate)

  ready_msg = ""
  while ready_msg.find("READY") == -1:
    time.sleep(0.2)
    ready_msg = arduino.readline().decode('utf-8')
    if len(ready_msg) > 0:
      print(ready_msg)
  
  command_list = [
                  "<temp;periodic;500>",
                  "<lighter;ignite;4000>"
                  ]

  for cmd in command_list:
    first_idx = cmd.find("<")
    last_idx = cmd.find(">")
    if (first_idx == -1 or last_idx == -1):
      print("Invalid command")
      continue
    if (first_idx + 1 == last_idx):
      print("Empty command")
      continue

    component = cmd[(first_idx+1):last_idx].upper()
    # print(component)
    if component == "CONDMETER":
      # conductivity = cond_meter.getConductivity()
      conductivity = 1434
      print("Conductivity reading: {} uS/cm".format(conductivity))
      continue
    SendCommand(arduino, cmd)
    PrintCommand(arduino)
    PrintResponse(arduino)



  arduino.close()
  
####################################################################################



if __name__ == '__main__':
  main()