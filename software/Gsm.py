import serial

com = input("Enter serial port: ")
baud = int(input("Enter baud rate: "))

port = serial.Serial(com,baud,timeout=1)

while True:
    
    command = input("Enter at command: ")
    port.write((command+"\r").encode("utf-8"))
    port.write(str.encode(chr(26)))

    s = port.read(100)
    if len(s)>0:
        print(s.decode("utf-8"))
