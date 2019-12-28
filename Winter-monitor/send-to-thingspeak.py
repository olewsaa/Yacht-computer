#!/usr/bin/env python

import sys
import os
import glob
import time
import urllib2
import math

baseURL='http://api.thingspeak.com/update?api_key=****************'
URL=baseURL
base_dir = '/sys/bus/w1/devices/'
sensors=glob.glob(base_dir + '28*')
j=0
T=[0,0,0]
for sensor in sensors:
	sens=sensor+'/w1_slave'
	raw = open(sens, "r").read()
	T[j] = float(raw.split("t=")[-1])/1000
	URL=URL+'&field'+str(j+1)+'='+str(T[j])
	j+=1
#	print(URL)

#print("Temperature sensor %d : %4.1f deg C" %(j, T))
#print("%d  %4.1f"  %(j, T))

pressure=open("/sys/bus/i2c/drivers/bmp280/0-0077/iio:device1/in_pressure_input","r").read()
# Correct for 1 m above sea level, scale height (8.314*278)/(0.029*9.82)
P=float(pressure)*10.0*math.exp(1.0/8160.0)
#print P,"hPa"


#print("Temperature sent ",T)
#print("Pressure sent ",P)

#print("Humidity")
dht11=" "
while dht11.find("True") < 0:
	dht11 = os.popen('python /home/ole/python/DHT/hum.py').read()
	time.sleep(2)

H=float(dht11[107:113])
#print(H)

#print baseURL+str(T)+'&field2='+str(P)+'&field3='+str(H)

# Send it to thingspeak
URL=URL+'&field4='+str(H)+'&field5='+str(P)
#print(URL)

f = urllib2.urlopen(URL)
f.read()
f.close()



