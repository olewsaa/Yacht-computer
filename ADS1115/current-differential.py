#!/usr/bin/env python

import time, socket
import Adafruit_ADS1x15


# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Create an ADS1115 ADC (16-bit) instance, use a parameter to set another
# address than default 0x48.

adc = Adafruit_ADS1x15.ADS1115(address=0x49)

# Choose a gain of 1 for reading voltages from 0 to 4.09V.
# Or pick a different gain to change the range of voltages that are read:
#  - 2/3 = +/-6.144V
#  -   1 = +/-4.096V
#  -   2 = +/-2.048V
#  -   4 = +/-1.024V
#  -   8 = +/-0.512V
#  -  16 = +/-0.256V
# See table 3 in the ADS1015/ADS1115 datasheet for more info on gain.
GAIN = 1
# Maximum voltage is somewhat above this, Vcc - 0.3 which could be 4.7 V if the
# supply voltage is 5V. 


# Note you can change the differential value to the following:
#  - 0 = Channel 0 minus channel 1
#  - 1 = Channel 0 minus channel 3
#  - 2 = Channel 1 minus channel 3
#  - 3 = Channel 2 minus channel 3

# Samples per second.
SAMPLES=8 

# Differential mode is the most sensible mode for Hall elements which outputs
# vcc/2 at zero voltage. Using a Vin- at Vcc/2 will yield zero voltage difference
# for zero current. A nice match.
#
# There are only 2 single ended channels and only two in differential mode.
# One might concider a MCP3008 to get 4 differential (but limited to 10 bits)
# channels.


zero = [0,1]
zero[0]=21 # Given in count from ADC [-32768, 32767], normally close to zero.
zero[1]=12
range = [0,1]
range[0]=8.4   # How many amp per volt,
range[1]=17.3  # slope of the Hall element response curve.


while True:
    for j in 0,1:
#  There are only two inputs using differential mode.      
        value=adc.read_adc_difference(j*3, gain=GAIN, data_rate=SAMPLES)
        u=(float(value-zero[j])/32768.0)*4.096
        i=u*range[j]
#        print "Channel : ",j,value,u,format(i,'.2f'),"A"
        
        if j == 0:
             SignalK='{"updates": [{"$source": "I2C.ADS1115","values":[ {"path": "electrical.ACS712.current","value":'+format(i,'5.3f')+'}]}]}'
             sock.sendto(SignalK, ('127.0.0.1', 55557))
        if j == 1:
            SignalK='{"updates": [{"$source": "I2C.ADS1115","values":[ {"path": "electrical.WCS1800.current","value":'+format(i,'5.3f')+'}]}]}'
            sock.sendto(SignalK, ('127.0.0.1', 55557))
            
    time.sleep(1)

    

