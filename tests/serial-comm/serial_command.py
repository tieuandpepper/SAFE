import serial
import time
import os
import sys

# comm_port = sys.argv[0]
comm_port = '/dev/tty.usbmodem14101'

arduino = serial.Serial(port = comm_port,  baudrate=115200, timeout=.1)

def write_read(x):
    arduino.write(bytes(x,  'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return  data


while True:
    num = input("Enter a number: ")
    value  = write_read(num)
    print(value)

