import serial

COMM_PORT = "COM14"
BAUD_RATE = 9600
ser = serial.Serial(COMM_PORT, baudrate=BAUD_RATE)