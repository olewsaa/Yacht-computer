# Temperature sensor 

## ESP-8266 breadboard layout

### 1-wire sensors 


I have used an ESP-12E NodeMCU 1.0 board from China for this
project. The software development environment is Arduino IDE which
relies on the (close enough) C programming language.

The temperature sensor project has [web pages](https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/temperature-monitoring) about the project with overview and pictures of the 
hardware in addition a description and ideas behind the setup.

The SignalK names selected should match the standard and what OpenPlotter recognise. These three are used for the current setup :
* propulsion.1.temperature
* environment.inside.engineRoom.temperature
* electrical.alternators.1.temperature

Using these labels OpenPlotter can pick them up and trigger alarms if any of these runs to high. 


![Breadboard](https://github.com/olewsaa/Yacht-computer/blob/master/img/Temperatures_bb.png 
"ESP-8266 breadboard layout")

The 1-wire sensors are powered by 3.3 Volt from the ESP-8266. There should be a pull up resistor from +3.3V to the signal wire, this is omitted on the figure, it works without for testing, but is not stable. Definitely not for longer cables and onboard with a lot of electric interference.

In the box all 1-wire terminals are connected in parallel, the power require 
two wires while all the signals travels on a shared single bus wire, hence the name
1-wire. It's a very simple setup and extra sensors can easily be added. Each sensor has
a unique serial number so there is no addressing issues.



