# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi



### ESP8266 Serial ports
The ESP8266 only have one serial port and this is the connection, upload
and debug port. In order to get an extra port a software port is neeed.
This is a standard library to eh arduino IDE, SoftwareSerial. This can
only handle 8 bits serial data. Since Seatalk uses 9 bot data values a
forked and modified library is used. Some issues were encountured, hence
a replacement file is made available.

The test for library and software is called "SowareSerialTest.ino".

