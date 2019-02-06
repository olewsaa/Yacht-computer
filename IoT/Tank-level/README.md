# Breadboard schematics

## ESP-8266 layout

The breadboard picture show the connections to where the NPN inductive sensors are connected.
I have used a ND 1205 isolating buck converter to make 5V from the boats 12V supply. More on 
this in the web pages about this project (https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/a-note-on-isolation) .

![Breadboard](https://github.com/olewsaa/Yacht-computer/blob/master/img/Tank-level_bb.png 
"ESP-8266 breadboard layout")

The pins named Data in the board have a different GPIO value, the table below show the mapping.

| GPIO pin    | Pin on board    |
| :---------: | :-------------: |
| GPIO 16     | D0              |
| GPIO 05     | D1              |
| GPIO 04     | D2              |
| GPIO 00     | D3              |
| GPIO 02     | D4              |
| GPIO 14     | D5              |
| GPIO 12     | D6              |
| GPIO 13     | D7              |
| GPIO 15     | D8              |
| GPIO 03     | RX              |
| GPIO 01     | TX              |

Some of the pins could not be used, GPIO 2 pin D4 is also used for transmit and when the 
sensor pull this up during init it fails to start the wifi. GPIO 00, pin D3 also has issues,
GPIO 02 and GPIP 01 are also RX and TX, hence pulling them up or down during init is not good. 
GPIO 16 also has limitations. The net result is that there is a limited number of pins that can 
be used for input of this kind.

