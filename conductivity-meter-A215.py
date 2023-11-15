# The mini-USB to USB comm only works with WINDOWS
# Default baud rate: 9600 | 8 data bits | Parity: none | Stop bits: 1 | flow control: none
# Terminate a command: CR - carriage return, ASCII 13 (0x0D)
# Characters in command: \n is ignored. lower case characters is converted to upper case internally.
# only 1 command is processed at a time
# Response "> " when the last command is processed and a new command can be entered
# COMMAND STRUCTURES: <Opcode> <Operands> <CR>
# 
import serial

class ConductivityMeter:
  def __init__(self, comm_port, brate=9600):
    self.baud_rate = brate
    # self.data_bits = 8
    # self.parity = None
    # self.stop_bits = 1
    # self.flow_control = None
    self.port = comm_port
    self.terminal = serial.Serial(port = self.port,  baudrate = self.baud_rate,timeout=10)
    self.terminal.close()
  
  def __str__(self):
    return f"A215 Conductivity Meter - {self.baud_rate} bits/s"

  def sendCommand(self, opcode:str, operands:list):
    msg = opcode
    for i in operands:
      msg = msg + i
    msg = msg + '\r'
    self.terminal.open()
    self.terminal.write(bytes(msg,  'utf-8'))
    self.terminal.close()
  
  def getData(self):
    self.terminal.open()
    data = self.terminal.read_until(expected='>')
    self.terminal.close()
    return data
  
meter = ConductivityMeter("COM15")
print(meter)
meter.sendCommand("GETLOG",[])
while 1:
  data = meter.getData().decode("utf-8")
  print(data)
  if data.find('>') != -1:
    break
