import serial
import time

port = "COM4"
ser = serial.Serial(port, 38400, timeout=0)   # open serial port
print("Hello world!")
print(ser.name)                               # check which port was really used

pseudo_temp = 1.00

while True:
    write_out = bytes("{\"Temperature\":\"%s\"}\n" % pseudo_temp, 'utf-8')
    print("Output %s" % write_out)
    ser.write(write_out)
    ser.flush()
    pseudo_temp = pseudo_temp + 1.00

    time.sleep(30)