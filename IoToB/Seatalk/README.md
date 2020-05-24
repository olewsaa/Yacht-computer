# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi



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

