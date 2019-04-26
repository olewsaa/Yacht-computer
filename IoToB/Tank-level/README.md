# Tank level sensors

## ESP-8266 breadboard layout

I have used an ESP-12E NodeMCU 1.0 board from China for this
project. The software development environment is Arduino IDE which
relies on the (close enough) C programming language.

The tank level has 
[web pages](https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/tank-level-monitor)
about the project with overview of the project and pictures, given here are source code and 
breadboad schematics.
The breadboard picture show the connections to where the NPN inductive
sensors are connected.  I have used a ND 1205 isolating buck converter
to make 5V from the boats 12V supply. More on
[isolation](https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/a-note-on-isolation)
in the web pages about this project.

The file  "Wifi-tank-level.ino" is the one used, the other Arduino sketches are older development  
versions included for reference.

![Breadboard](https://github.com/olewsaa/Yacht-computer/blob/master/img/Tank-level_bb.png 
"ESP-8266 breadboard layout")

The pins named Data in the board have a different GPIO value, the table below show the mapping.

| GPIO pin    | Pin on board    | Additional use |
| :---------: | :-------------: | :------------: |
| GPIO 16     | D0              | USER / WAKE    |
| GPIO 05     | D1              |                |
| GPIO 04     | D2              |                |
| GPIO 00     | D3              | FLASH          |
| GPIO 02     | D4              | TXD1           |
| GPIO 14     | D5              | HSCLK          |
| GPIO 12     | D6              | HMISO          |
| GPIO 13     | D7              | RXD2 / HMOSI   |
| GPIO 15     | D8              | TXD2 / HCS     |
| GPIO 03     | RX              | RXD0           |
| GPIO 01     | TX              | TXD0           |

Some of the pins could not be used, GPIO 2 pin D4 is also used for transmit and when the 
sensor pull this up during init it fails to start the wifi. GPIO 00, pin D3 also has issues,
GPIO 02 and GPIP 01 are also RX and TX, hence pulling them up or down during init is not good. 
GPIO 16 also has limitations. The net result is that there is a limited number of pins that can 
be used for input of this kind. Hence pins D2,D5,D6 and D7 are used as digital input pins. 

