# Yacht-computer project
## Yacht-server background

Every yacht need to have a server (or servers) on board. There are
many need to be fulfilled, of which OpenPlotter cover a significant
fraction, OpenCPN and SignalK are prime examples. There are still gaps
to be filled. Monitoring of equipment like engine, tank levels,
temperatures, voltage and current etc etc. The list is long. In
addition comes the need to a server to handle the comunication and a
file server on board. A central place to dump all pictures and video
from cameras and modiles and to stock up these with audiobooks and
videos.

The Raspberry Pi or similar types are ideal suited for this purpose,
very small and affordable which mean that severel servers can be
deployed without high cost og power cunsumption.

The goal is to replace as much as possible of the yacht instruments
with servers (Raspberry Pi etc) and laptops, pads and mobile
phones. This part her focuses on interfacing sensors and such with the
Rasperry Pi.  I keep a [web page about this
project](https://sites.google.com/site/olewsaa/yacht-server).

## OpenPlotter and OpenCPN

The server part is running OpenPlotter which takes care of almost all the server related functions and services. The server hosts SignalK which provide web server that allow display of data on web pages both local and for any client that want to connect. 

The server also run OpenCPN as a chart plotter on board. The OpenCPN accept various signals from OpenPlotter and display GPS positions to update the boats position and accept AIS NMEA signals again to be plotted.  All of this is nicely run on a Raspberry Pi.

## HF/SSB radio control

The pat directory contain software used to control the HF/SSB radio, including winlink email which uses the pat software. 

## Wifi gateway - wifi2wifi

This project is to use a Pi as a gateway to the internet. A high gain wifi antenna that usually connect using USB connect to a remote access point. Then the gateway provieds a local network onboard to which all client on board can connect to. 

## Communication server  

Another Raspberry Pi is set up to handle communication as this might prove to much for the one runnng both Openplotter and OpenCPN. This is a separate [project](https://sites.google.com/site/olewsaa/yacht-server/raspberry-pi-as-a-router-gateway). I have taken some ideas from the famous RedBox, which can do very much more than my litte project. 

## File server

A little file server with Bluetooth interface ta facilitate easy transfer of pictures and video from the mobiles, teenagers are happy to dump selected files onto a file server, but refuse to let a cable enter their phones. In addtion the file server can host autobooks, music and films. A large USB attached SSD is a good way to get high storage capacity.   

## Server and IoT sensors

Files and documents related to the Yacht computer project. This git
repository contain only the actual files used, the project is
described
[here](https://sites.google.com/site/olewsaa/yacht-server-with-raspberry)
and for the versions installed or in process of being installed
[here](https://sites.google.com/site/olewsaa/yacht-server).

## IoToB
The Internet of Things directory contains files for the IoT approach to the project. 
Which has become the major part of this project. 

## Development of data systems and IoT sensors 

Some of this might find its way into the Openploter project (but
already it provides a platform for collection of information from the
networked(wifi) sensors on board, SignalK server end associated web
server.  Some are demo files showing how to use a type of sensor or
other feature. However, this project it basically to show how you
instrument the boat with simple and cost effective solutions.

Personally I have opted for a Internet of Things on Board (ioToB)
where the sensors are autonomous units that sends off the data to the
central server over wifi (SignalK server that comes with the
OpenPlotter distro).

## Learning to use chips and sensors

Some subproject underway with inital testing include:

1. PCF8574 is a digital input output chip. Used to measure water in tanks.
2. MCP3008 is a 10 bit Analog to Digital Converter (ADC). Has many uses like voltage and current.
3. ADS1115 is a 16 bit ADC, a high precision ADC for usage where more than 10 bit is needed. 
4. DS18B20 is a one wire temperature device, a nice device for monitoring temperatures.
5. ESP-8622 is the IoToB chip/board of choice. Small, simple, almost costless.
6. ESP-32 is one step up, far more connections and still small and very cost effective.

