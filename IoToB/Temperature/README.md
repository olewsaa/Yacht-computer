# Temperature sensor 

## ESP-8266 breadboard layout

### 1-wire sensors 


I have used an ESP-12E NodeMCU 1.0 board from China for this
project. The software development environment is Arduino IDE which
relies on the (close enough) C programming language.

The temperature sensor project has [web pages](https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/temperature-monitoring) about the project with overview and pictures of the 
hardware in addition a description and ideas behind the setup.




![Breadboard](https://github.com/olewsaa/Yacht-computer/blob/master/img/Temperatures_bb.png 
"ESP-8266 breadboard layout")

The 1-wire sensors are powered by 3.3 Volt from the ESP-8266.

In the box all 1-wire terminals are connected in parallel, the power require 
two wires while all the signals travels on a shared single bus wire, hence the name
1-wire. It's a very simple setup and extra sensors can easily be added. Each sensor has
a unique serial number so there is no addressing issues.



