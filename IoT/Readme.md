Source code and documentaton related to the Internet of Things onboard and for the Yacht server project.

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
