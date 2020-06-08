# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi

### Electric interface

The Seatalk uses a different signal level codeing than the serial
communication fo the normal Arduino/ESP8266. The common TTL serial
signal level are 0V for logic 0 and +3.3 or 5V for logic 1. 
See the the [Thomas Knauf Seatalk page](http://www.thomasknauf.de/rap/seatalk1.htm).

This require a level convertion from +12V/3.3V to 0V/0V
in order to process the signal using thre ESP8222. 

The figure show how the level convertion is done using a single
transistor, the simulation show the voltage levels and that the output
is safe to output to the pins of the ESP6622. The Seatalk data source
is replaced by a signal generator to simulate the Seatalk data.

![figure](https://github.com/olewsaa/Yacht-computer/blob/master/IoToB/Seatalk/Seatalk-to-ESP8266_schem.png)

And the electric signal simulated to show the 3.3V signaling level.

![figure](https://github.com/olewsaa/Yacht-computer/blob/master/IoToB/Seatalk/Seatalk-Grapher.png)







