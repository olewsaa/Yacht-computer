# Internet of Things on Board

Source code and documentaton related to the Internet of Things onboard
(IoToB) and for the Yacht server project.

The newest first, further down this text is older files and attempts. 

## ESP-8266 modules 

The ESP-8266 module is remarkable litte device. With wifi built in and
most of the software stack for this built in it has gained enormous
popularity amoung makers. With a single core and limited memory the
size of application it can run is limited. 

The breakout ESP‑12E board also known as
[ESP‑12E](https://en.wikipedia.org/wiki/NodeMCU) is well suited on
breadboard and on small circuit boards. It has a good selection of IO
pins and a single Analog input with a 10 bit A/D.

## ESP-32 based modules

ESP32-WROOM-32 The best known model of
[ESP32](https://en.wikipedia.org/wiki/ESP32) is the
[NodeMCU-ESP32S](https://docs.zerynth.com/latest/official/board.zerynth.nodemcu_esp32/docs/index.html).
The ESP32 is a more versatile module with two cores, more memory and a
lot more GPIO pins and a range of 12 bits ADCs built in. This together
with the more powerfull 3.3V outlet makes it more suited to many
Internet of things onboard (IoToB). The WiFiClient-U-and-I-SignalK in
version 3 is a ESP-32 version. There are distinct differences in the
handling of UDP trasactions. This prototype uses two ADS1115 for
current measurements and the built in ADC for voltage measurement.

### Compare ESP8266 vs. ESP32
There is a [nice page comparing](https://makeradvisor.com/esp32-vs-esp8266) the performance 
and other relevant characteristics of the two.
The ESP32 suppport floating point, but performance is limited. This 
[blog](https://blog.classycode.com/esp32-floating-point-performance-6e9f6f567a69) entry 
show what to expect. 


## Projects using the two modules

### Temperature 
WiFiClient-TempSignalK is a sketch where an ESP-8266 module is used to gather the temperatures from the one-wire bus and forward them to SignalK. It uses a simpler 8266 based ESP-12E module as this module is more than capable of performing this task. The 3.3V power converter have no problem feeding the DS18B20 sensors.

### Tank level
Tank level monitoring by means of inductive sensors. Thes eon off sensors triggers IO pins on the ESP12 module and calculate a level number that is sent to SignalK over wifi. 

### Current and voltage
WiFiClient-U-and-I-SignalK.ino is a sketch where the ADS1115 is
connected to the ESP-12E board. The Adafruit library for ADS1115 is
used. GPIO pin 4 and 5 are connected to I2C SDA and SLC, this is the
default.  The ADS1115 measure current using Hall element sensors that
provide +Vdd/2 at zero current and 0 voltage at maximum negative
current and close to ful voltage at maximum positive current. Hence
the differential mode.  The ESP8266/12-E has a single analog input,
this is used to measure the 12V voltage.  Both current and voltage is
then sent to the SignalK server.

### Voltage
Initial test is to build a IoT voltmeter that accept 0-18 Volt and
send a string of data to the SignalK server on the OpenPlotter
computer. File is : WiFiClient-analog-SignalK.ino , this is rather
simple sketch with not very elegant code.


### SignalK test
The Python scripts are test scripts to send and receive data from the
ESP8266 to check if things work correct and can be sent to SignalK.

