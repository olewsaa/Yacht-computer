# Interfacing to CANbus / NMEA2000

## Introduction
Interfacing CANbus (Controller Area Network) or NMEA2000 to
OpenPlotter allows you to integrate your marine navigation and
communication systems with your OpenPlotter setup. CANbus and NMEA2000
are both widely used communication protocols in the marine
industry. They enable devices like chartplotters, GPS receivers,
sensors, and other marine instruments to exchange data and work
together seamlessly.

The CANable device is a popular USB-to-CAN adapter that allows you to
connect your computer to a Controller Area Network (CAN) bus. It is a
compact and affordable device that provides a simple way to interface
with CAN networks for various applications, including automotive,
industrial, and marine systems.

The CANable device is based on the Microchip MCP2515 CAN controller
and MCP2551 CAN transceiver. It offers a USB interface for easy
connectivity to your computer and supports both CAN 2.0A (11-bit
identifier) and CAN 2.0B (29-bit identifier) protocols.

With the CANable device, you can integrate CAN bus functionality into
your OpenPlotter setup. By connecting the CANable adapter to your
Raspberry Pi running OpenPlotter, you can interface with CAN-enabled
marine devices, retrieve data from the network, and utilize it within
the OpenPlotter ecosystem. This enables you to access and process data
from instruments like chartplotters, engine monitors, sensors, and
more, enhancing your marine navigation, monitoring, and automation
capabilities.


## Using the Canable device

The [canable](https://canable.io/) can translate the CANbus messages and data to a serial Linux device. 
This device will manifest itself as a /dev/ttyACM0 (og 1,2 etc if many devices are connected). 

In openplotter dialog window this device could be selected, but that is nor always a good solution as the 
ttyACMx number can change. A far better option is to make a udev entry and give the canable device a 
proper name in the /dev/ directory, my coice is 'canable'.

### Making a udev antry

To make a new udev entry one need to find the right numbers for the device. Assuming that the canable device
came up as /dev/tty ACM0 :
```
udevadm info --query=all --name=/dev/ttyACM1 | grep ID_VENDOR_ID 
E: ID_VENDOR_ID=ad50
udevadm info --query=all --name=/dev/ttyACM1 | grep ID_MODEL_ID
E: ID_MODEL_ID=60c4
```
These numbers differ from the ones given on the canable [web page](https://canable.io/updater/udev.html).

The udev rules are found at : /lib/udev/rules.d/ and the file 10-local-rpi.rules is used for local rules.
Append the line :
```
ATTRS{idVendor}=="ad50", ATTRS{idProduct}=="60c4", SYMLINK="canable%n"
```
to the /lib/udev/rules.d/10-local-rpi.rules and run the command
```
udevadm trigger
```
if the Vendor and Product match the values found when the device report in you should now have
a new device in the dev directory, /dev/canablex (x might be 0, 1, 2, I got /dev/canable1 and /dev/canable2).
They are :
```
root@OpenPlotter:/lib/udev/rules.d# ls -l /dev/cana*
lrwxrwxrwx 1 root root      7 juni   3 12:18 /dev/canable1 -> ttyACM1
lrwxrwxrwx 1 root root     15 juni   3 12:18 /dev/canable2 -> bus/usb/001/004
```
One is a link to the USB port while the other is a link to the ttyACM1 device. I use canable1 in 
Openplotter.


