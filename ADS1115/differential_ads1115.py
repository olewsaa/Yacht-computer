#!/usr/bin/env python

# This file is part of Openplotter.
# Copyright (C) 2015 by sailoog <https://github.com/sailoog/openplotter>
# 					  e-sailing <https://github.com/e-sailing/openplotter>
# Openplotter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# any later version.
# Openplotter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Openplotter. If not, see <http://www.gnu.org/licenses/>.

import socket, time, RTIMU, math, csv, datetime, subprocess, sys
import Adafruit_ADS1x15 # Use Adafruit library for ads1115.
from classes.paths import Paths
from classes.conf_analog import Conf_analog


paths=Paths()
toolspath=paths.op_path + '/tools'
home=paths.home


# Quick and dirty hack for differential measurements using ADS1115        
# Put the settings here before moving them  to config files.
GAIN_I = 0 # 1 = +/-4.096V 2 = +/-2.048V 4 = +/-1.024V
GAIN_U = 0 #

# Note you can change the differential value to the following:
#  - 0 = Channel 0 minus channel 1
#  - 1 = Channel 0 minus channel 3
#  - 2 = Channel 1 minus channel 3
#  - 3 = Channel 2 minus channel 3
        
SAMPLES_I=0
SAMPLES_U=0
# ADC conversion time (in samples/second). 
 
OFFSET_I=0
OFFSET_U=0
# ADC count offset, related to the intercept variable.


# Define functions to get the current i and voltage u.
# Eventually the settings parameters will be read from the
# config file.

def get_i():
    val=adc.read_adc_difference(0, gain=GAIN_I, data_rate=SAMPLES_I)
    val=val-OFFSET_I
#  Large Hall element, http://www.yhdc.com/en/product/379/ 50A version.
#  Pins from edge Vref Vout 0V Vdd
#  Linear regression, cal using multimeter.
#  y (current) = -0.52372 + 0.0025559 * x (counts)

    i = -0.52061 + 0.0024236 * val
    if abs(i)<0.1:  # Cheating, but avoiding non zero values at zero.
        i=0.0       
    return i

def get_u():
    val=adc.read_adc_difference(3, gain=GAIN_U, data_rate=SAMPLES_U)
    val=val-OFFSET_U
    # 10k/54k=0.185185 ; rounded to 0.1841 after cal with voltmeter.
    u=(((float(val)/float(32768))*4.096)/0.1841)
    if u<0.000001: # So close to zero that we flush to zero.
        u=0.0
    return u
    



if len(sys.argv)>1:
	if sys.argv[1]=='settings':
#		print toolspath+'openplotter_analog.conf'
		subprocess.Popen(['leafpad',toolspath+'/openplotter_analog.conf'])
	exit
else:
        conf_analog=Conf_analog()
        
# Instanciate a ADS1115 class object.
# Note you can change the I2C address from its default (0x48), and/or the I2C
# bus by passing in these optional parameters:
#adc = Adafruit_ADS1x15.ADS1015(address=0x49, busnum=1)
        adc = Adafruit_ADS1x15.ADS1115()
        
        
	a_index = [4,5]
	a_value = [0.0,0.0,0.0,0.0,0.0,0.0]
	index=3
	active=['0','0','0','0','0','0']
	SK_name=['0','0','0','0','0','0']

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	poll_interval = 1
	rate_analog = 1
	ADS1115='ADS1115_'

	for i in a_index:
		active[i] = conf_analog.get(ADS1115+str(i), 'active')=='2'
                print("i :", i,"active ",active[i])
		if active[i]:
			if 0==conf_analog.has_option(ADS1115+str(i), 'sk_name'):
				conf_analog.set(ADS1115+str(i), 'sk_name','0')

			SK_name[i] = conf_analog.get(ADS1115+str(i), 'sk_name')

                        if i==4:
                            if conf_analog.has_option(ADS1115+str(i), 'gain')!=0:
                                GAIN_I=conf_analog.getint(ADS1115+str(i), 'gain')
                            if conf_analog.has_option(ADS1115+str(i), 'samples')!=0 :
                                SAMPLES_I=conf_analog.getint(ADS1115+str(i), 'samples')
                            if conf_analog.has_option(ADS1115+str(i), 'offset')!=0 :
                                OFFSET_I=conf_analog.getint(ADS1115+str(i), 'offset')
                        if i==5:
                            if conf_analog.has_option(ADS1115+str(i), 'gain')!=0:
                                GAIN_U=conf_analog.getint(ADS1115+str(i), 'gain')
                            if conf_analog.has_option(ADS1115+str(i), 'samples')!=0 :
                                SAMPLES_U=conf_analog.getint(ADS1115+str(i), 'samples')
                            if conf_analog.has_option(ADS1115+str(i), 'offset')!=0 :
                                OFFSET_U=conf_analog.getint(ADS1115+str(i), 'offset')
                        
                        #print("GAIN_I ",GAIN_I," GAIN_U ",GAIN_U)
                        #print("SAMPLES_I ",SAMPLES_I," SAMPLES_U ",SAMPLES_U)
                        #print("OFFSET_I ",OFFSET_I," OFFSET_U ",OFFSET_U)
                            


        print(a_index[0], active[0], SK_name[0])
        print(a_index[1], active[1], SK_name[1])        
	tick_analog=time.time()
#        print(poll_interval)
	while True:
		tick2=time.time()
		time.sleep(poll_interval*1.0/1000.0)
		#GENERATE	

		if tick2-tick_analog > rate_analog:
			tick_analog=time.time()

			list_signalk_path1=[]
			list_signalk1=[]

			for i in a_index:
				if active[i]:	
					list_signalk_path1.append(SK_name[i])
					list_signalk1.append(str(a_value[i]))
#                        print(SK_name[i])         
			if list_signalk1:
				SignalK = '{"updates": [{"$source": "I2C.ADS1115","values":[ '
				Erg=''
				for i in range(0,len(list_signalk1)):
					Erg += '{"path": "'+list_signalk_path1[i]+'","value":'+list_signalk1[i]+'},'
				SignalK +=Erg[0:-1]+']}]}\n'
				sock.sendto(SignalK, ('127.0.0.1', 55557))

		else:
			index+=1
			if index>5: index=4

                        if index == 4:
                                a_value[index]=get_i()
                        else:
                                a_value[index]=get_u()                           

                        print(index,a_value[4],a_value[5])
