import serial
import time

# COMM_PORT = "COM13"
COMM_PORT = "/dev/tty.usbmodem14201"
BAUD_RATE = 9600
data = b'abcd'
ser = serial.Serial(COMM_PORT, baudrate=BAUD_RATE, timeout=5)
# ser.open()
ser.write(data)
time.sleep(1)
data = ser.read_all()
print(data)
ser.close()