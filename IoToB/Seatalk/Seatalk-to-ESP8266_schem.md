# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi

### Electric interface

The Seatalk uses a different signal level coding than the serial
communication fo the normal Arduino/ESP8266, +12V and 0V. The common
TTL serial signal level are 0V for logic 0 and +5V or 3.3V for
logic 1.  See the the [Thomas Knauf Seatalk
page](http://www.thomasknauf.de/rap/seatalk1.htm) which is the reference
page for Seatalk information. This require a level convertion from
+12V to +3.3V in order to process the signal using the
ESP8222 which uses uses 3.3V (Arduino uses 5V).

#### Simulation

The figure show how the level convertion is done using a single none
inverting transistor stage, the simulation show the voltage levels and
that the output is safe to output to the pins of the ESP8622. The
Seatalk data source is replaced by a signal generator to simulate the
Seatalk data.

![figure](https://github.com/olewsaa/Yacht-computer/blob/master/IoToB/Seatalk/Seatalk-to-ESP8266_schem.png)

And the electric signal simulated to show the 3.3V signaling level.

![figure](https://github.com/olewsaa/Yacht-computer/blob/master/IoToB/Seatalk/Seatalk-Grapher.png)







