#!/usr/bin/env python
#import the library / Import des librairies
from pyA20.gpio import gpio
from pyA20.gpio import port
from time import sleep

#initialize the gpio module / initialise le GPIO
gpio.init()

#setup the port (same as raspberry pi's gpio.setup() function)
#Configure la broche PG8 
gpio.setcfg(port.PG8, gpio.OUTPUT)

gpio.output(port.PG8, gpio.HIGH)
#gpio.output(port.PG7, gpio.LOW)

