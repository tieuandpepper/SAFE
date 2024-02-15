# Default baud rate: 9600 | 8 data bits | Parity: none | Stop bits: 1 | flow control: none
# Terminate a command: CR - carriage return, ASCII 13 (0x0D)
# Characters in command: \n is ignored. lower case characters is converted to upper case internally.
# only 1 command is processed at a time
# Response "> " when the last command is processed and a new command can be entered
# COMMAND STRUCTURES: <Opcode> <Operands> <CR>
# 
import serial
import time

req_stat = b'\xcc\x00\x3e\x00\x00\xDD\xE7\x01'
terminal = serial.Serial(port = '/dev/tty.usbserial-1140', baudrate=9600, timeout=50)
terminal.close()
terminal.open()
terminal.write(req_stat)
# terminal.close()
# time.sleep(0.1)
# terminal.open()
resp = terminal.read(8)
terminal.close()
print(resp)
print("done")