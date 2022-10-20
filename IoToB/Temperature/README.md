# Temperature sensor 

## Computer system - ESP32

### Introduction
The DS18B20 sensors are not too happy with high temperature environment.
They have a high failure rate which is no good. Maybe I got a bad batch, but
the sensors are all different. NTC is a better option. NTCs are also used 
in the automotive sector which make them as better alternative. It require
analog to digital converting, e.g. reading voltage which the ESP8266 dont't
have. Using the ESP32 is a better option with several analoge inputs.

### NTC 10k sensors
The NTC sensors come in many packages, some are sensors with M8 threads which
fit into spare threaded sockets in the engine block and screw on lug terminals.

### Calibration of the NTC sensors
The resistance do not vary linearly with temperature. The curve need
to be obtained by using table values or by subjecting the sensors to
different temperatures and noting down the resistance.

I used table values and arrived at a polynomial fitting, polynomials
are simple and fast to evaluate. To get the error low I used a fourth
degree polynomial, with the downside that I needed double presicison
vaiables (64 bits float), the ESP32 don't support this in hardware. 
However, it's only done every 5 second (my current measurement frequency).

As I run into stability issues with the 1-wire sensors I added a restart every N minutes.
While not a power cycle restart it's a full reboot of the ESP32.

### Openplotter and SignalK
The SignalK server can accept UDP input at a user selectable port. The ESP8266
is using 55557 hence the ESP32 could use 55558 or another suitable port. This 
just need to be entered in the data source section of SignalK. The web 
interface makes this easy. 


## Computer system - ESP-8266

### 1-wire sensors 

I have used an ESP-12E NodeMCU 1.0, an ESP8266 board from China for
this project. The ESP8266 well suited for this task, 1-wire and wifi
which is all that's needed. The software development environment is
Arduino IDE which relies on the C (close enough) programming language.

### Use case - on board
The temperature sensor project has [webpages]
(https://sites.google.com/site/olewsaa/yacht-server-with-raspberry/temperature-monitoring)
about the project with overview and pictures of the hardware in
addition a description and ideas behind the setup.

### OpenPlotteer and SignalK 

The SignalK names selected should match the standard and what OpenPlotter recognise. 
These three are used for the current setup :
* propulsion.1.temperature
* environment.inside.engineRoom.temperature
* electrical.alternators.1.temperature.warnUpper 

Using these labels OpenPlotter can pick them up and trigger alarms if
any of these runs to high. OpenPlotter can also translate to NMEA
sentences for export to openCPN.

### Setup and implementation

![Breadboard](https://github.com/olewsaa/Yacht-computer/blob/master/img/Temperatures_bb.png 
"ESP-8266 breadboard layout")

The 1-wire sensors are powered by 3.3 Volt from the ESP-8266. There
should be a pull up resistor from +3.3V to the signal wire.  My setup
is a star topology which is not really in line with best practice. The
pullup resistor is lowered to 2.2k as the three cables introduce a lot
of capacitance and inductance. In addition to limit the impedance
misfit and reflection problems on the transmission line a serial
resistor of 100 ohms is inserted in each line. So far these measures have
worked and the system now seem to be stable.

In the box all 1-wire terminals are connected in parallel, but with
the serial resistor mentioned above, the power require two wires while
all the signals travels on a shared single bus wire, hence the name
1-wire. It's a very simple setup and extra sensors can easily be
added. Each sensor has a unique serial number so there is no
addressing issues.

### Experience

The box was placed inside the stairway obove the engine, but outside of the 
isolation. While not 70°C like inside the engine room, hot enough for the B1205 
converter to overheat. The box is now placed in the technical room with a junction
inside the machine room and a 3 wire cable run to the box.







