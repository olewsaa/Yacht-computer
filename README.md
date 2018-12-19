# Yacht-computer project
## Yacht-computer background

The goal is to replace as much as possible of the yacht instruments with servers (Raspberry Pi etc) and laptops, pads and mobile phones. This part her focuses on interfacing sensors and such with the Rasperry Pi.

The server part is running OpenPlotter which takes care of almost all the server related functions and services. The server hosts SignalK which provide web server that allow display of data on web pages both local and for any client that want to connect. 

The server also run OpenCPN as a chart plotter on board. 

## Communication server 

Another Raspberry Pi is set up to handle communication as this might prove to much for the one runnng both Openplotter and OpenCPN. This is a separate project [https://sites.google.com/site/olewsaa/yacht-server/raspberry-pi-as-a-router-gateway] . I have taken some ideas from the famous RedBox, which can do very much more than my litte gateway.

## Server and IoT sensors

Files and documents related to the Yacht computer project. This git repository contain only the actual files used, the project is describer at [https://sites.google.com/site/olewsaa/yacht-server-with-raspberry]
and for the versions installed or in process of being installed [https://sites.google.com/site/olewsaa/yacht-server] 

## IoToB
The Internet of Things directory contains files for the IoT approach to the project. 
Which has become the major part of this project. 

## Development of data systems and IoT sensors 

Some of this might find its way into the Openploter project, some are demo files showing how to use a 
type of sensor or other feature. Personally I have opted for a Internet of Things on Board (ioToB)
where the sensors are autonomous units that sends off the data to the central server over wifi.

## Learning to use chips and sensors

Some subproject underway with inital testing include:

1. PCF8574 is a digital input output chip. Used to measure water in tanks.
2. MCP3008 is a 10 bit Analog to Digital Converter (ADC). Has many uses like voltage and current.
3. ADS1115 is a 16 bit ADC, a high precision ADC for usage where more than 10 bit is needed. 
4. DS18B20 is a one wire temperature device, a nce device for monitoring temperatures.

