#!/usr/bin/env python


# Aug 2017  OWS
# Adapted to work with the PCF8574A based module.  

import smbus, time, socket

# Get I2C bus
bus = smbus.SMBus(1)

# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# PCF8574  address, 0x20(32)
# PCF8574A address, 0x38(56)
PCF8574A=0x38
#		0xFF	All pins configured as inputs
bus.write_byte(PCF8574A, 0xFF)

time.sleep(0.5)

# PCF8574A address, 0x38(56)
# Read data back, 1 byte
data = bus.read_byte(0x38)

num_sensors = 3
interval = 1.0/float(num_sensors)

SignalK=""
while True:
    data = bus.read_byte(0x38)
    data = (data & 0xFF)
# Pin 2,3 and 4 are used for sensors, 1/3, 2/3 and full.
    for i in range(2, 4) :
        level=0
        if data & (2 ** 2) == 0 :
            level=level+interval
        if data & (2 ** 3) == 0:
            level=level+interval
        if data & (2 ** 4) == 0:
            level=level+interval    
    bus.write_byte(0x38,0xff)

    SignalK='{"updates": [{"$source": "I2C.PCF8574A","values":[ {"path": "tanks.freshWater.*.currentLevel","value":'+format(level,'5.3f')+'}]}]}'
    sock.sendto(SignalK, ('127.0.0.1', 55557))
    time.sleep(1)
        

