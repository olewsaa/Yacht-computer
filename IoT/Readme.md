Source code and documentaton related to the Internet of Things onboard (IoToB) and for the Yacht server project.

The newest first, further down this text is older files and attempts. 

The ESP-32 module is a more versatile module with many ADCs built in. This together with the more powerfull 3.3V 
outlet makes it more suited to many Internet of things onboard (IoToB). The WiFiClient-U-and-I-SignalK in version 3 
is a ESP-32 version. There are distinct differences in the handeling of UDP trasactions. This prototype uses two
ADS1115 for current measurements and the built in ADC for voltage measurement.

WiFiClient-TempSignalK is a sketch where an  ESP-8266 module is used to gather the temperatures from the one-wire bus and forward them to SignalK. It uses a simpler 8266 based ESP-12E module as this module is more than capable of performing 
this task. The 3.3V power converter have no problem feeding the DS18B20 sensors.

WiFiClient-U-and-I-SignalK.ino is a sketch where the ADS1115 is connected to the ESP-12E board. The Adafruit library 
for ADS1115 is used. GPIO pin 4 and 5 are connected to I2C SDA and SLC, this is the default. 
The ADS1115 measure current using Hall element sensors that provide +Vdd/2 at zero current and 0 voltage at 
maximum negative current and close to ful voltage at maximum positive current. Hence the differential mode.
The ESP8266/12-E has a single analog input, this is used to measure the 12V voltage. 
Both current and voltage is then sent to the SignalK server. 


Initial test is to build a IoT voltmeter that accept 0-18 Volt and send a string of data to the SignalK server 
on the OpenPlotter computer. File is : WiFiClient-analog-SignalK.ino , this is rather simple sketch with 
not very elegant code. 

The Python scripts are test scripts to send and receive data from the ESP8266 before it correct and can 
be sent to SignalK.