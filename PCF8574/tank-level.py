# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# PCF8574
# This code is designed to work with the PCF8574_LBAR_I2CL I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# PCF8574  address, 0x20(32)
# PCF8574A address, 0x38(56)
PCF8574A=0x38
#		0xFF	All pins configured as inputs
bus.write_byte(PCF8574A, 0xFF)

time.sleep(0.5)

# PCF8574A address, 0x38(56)
# Read data back, 1 byte
data = bus.read_byte(0x38)  # Blinking the LED at port 1.
time.sleep(0.2)

# Convert the data
# data = (data & 0xFF)

num_sensors = 3
interval = 1.0/float(num_sensors)

while True:
    data = bus.read_byte(0x38)
    data = (data & 0xFF)
    bus.write_byte(0x38,0xfe)
    for i in range(2, 4) :
        level=0
        if data & (2 ** 2) == 0 :
            level=level+interval
        if data & (2 ** 3) == 0:
            level=level+interval
        if data & (2 ** 4) == 0:
            level=level+interval    
    print 'Tank level : ',format(level,'5.3f')
    print 
    bus.write_byte(0x38,0xff)
    time.sleep(1)
    
