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
data = bus.read_byte(0x38)


# Convert the data
data = (data & 0xFF)

while True:
    data = bus.read_byte(0x38)
    data = (data & 0xFF)
    bus.write_byte(0x38,0xfe)
    for i in range(0, 8) :
#        print 'data', bin(data),' - ', bin(data & (2**i))
#        print("{0:b}".format(data),' - ',"{:#010b}".format(data&(2**i)))
#        print"{0:b}".format(data),' - ',"{:08b}".format(data&(2**i)),(data & (2 ** i))%data
        if (data & (2 ** i)) == 0 :
            print "I/O Pin %d State is LOW" %(i)
        else :
            print "I/O Pin %d State is HIGH" %(i)
            time.sleep(0.2)
    print 'done'
    bus.write_byte(0x38,0xff)
    time.sleep(1)
    
