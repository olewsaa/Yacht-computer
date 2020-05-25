# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi

### Introduction
Seatalk data are sendt over a special single wire propriatary bus and
in a special Seatalk format. The electrical sigalling is using 12V and null Volt.
This need to be translated to the TTL 3.3V level for the ESP8266 to read. 
The Setalk messages need to be read (in 9 bit format) and decoded. The decoded data 
are then transmitted over Wifi to the SignalK server and displayed by any SignalK
web application.

### ESP8266 are well suited
The ESP8266 are well suited to this work, it handle wifi comunication well and 
can be programmed to use pins to look like a serial port and have enough 
processor capacity to read and decode the messages. 

The ESP8266 are also easy to program with full support in the well known 
Arduiono IDE.


### ESP8266 Serial ports
The ESP8266 only have one serial port and this is the connection, upload
and debug port. In order to get an extra port a software port is neeed.
This is a standard library to eh arduino IDE, SoftwareSerial. This can
only handle 8 bits serial data. Since Seatalk uses 9 bot data values a
forked and modified library is used. Some issues were encountured, hence
a replacement file is made available.

The test for library and software is called SoftwareSerialTest.ino and is
a simple script adapted from the examples to test the software serial port.

The naming of the ports for D5 and D6 are the same marking as in the 
board. The 14 and 12 refer to the GPIO ports. The first pin argument is the Rx 
and the second the Tx. Hence the call:
swSer.begin(BAUDRATE, D5, D6, SWSERIAL_9N1, false, 95, 11);
would set up D5 (GPIO 14) as the receive pin and D6 (GPIO 12) as the transmit pin.


The repo for the 9 bit library is found here:
https://github.com/ionini/espsoftwareserial

Some adaptation are neeed, and information about this is found in the file UPDATE.md 
within my version of the SoftwareSerial directory. In addtion a working copy of the 
SoftwareSerial 9bit files.

