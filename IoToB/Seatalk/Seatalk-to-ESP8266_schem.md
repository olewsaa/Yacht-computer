# Yacht-computer project

## Raymarine Seatalk 1 to SignalK over Wifi

### Electric interface

The Seatalk uses a different signal level codeing than the serial
communication fo the normal Arduino/ESP8266. The common TTL serial
signal level are 0V for logic 0 and +3.3 or 5V for logic 1. Seatalk
uses +12 Volt for logic 0 and 0V for logic 1. See the the 
[Thomas Knauf Seatalk page](http://www.thomasknauf.de/rap/seatalk1.htm).

This require a level convertion from +12V/0V to 0V/3.3V and inverting
in order to process the signal using thre ESP8222. The 0V signal need
to be inverted to +3.3V while the +12V need to inverted to 0V. This is
done by a simple inverter circuit using one transistor.





