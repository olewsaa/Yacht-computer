# Position and Time

## Introduction
Position and time are always of interest. There are several sources available
onboard:
1. The internet provide time from ntp servers if connected to the net.
2. The Chart plotter (when turned on) provide both time and position via NMEA2000, SignalK and OpenPlotter.
3. Then no other source is available an attached USB GPS will provide the Yacht computer with both time and position.

As one need to save power when at anchor, the sailing instruments and chart 
plotter are ften turned off. The yacht computer is very handy to have on as it 
record air pressure. Wind require the instruments to be on, but not the 
chart plotter. The simple USB GPS "mouse" will provide position and time signal
to the yacht computer which in turn can provide time and position to any other 
computer (the ham radio one) and any laptops or mobile phones.  A simple 
[u-blocx](https://www.u-blox.com) 7th. generation is enough to privide millisecond
precisision timing. A newer [8th. generation](https://www.u-blox.com/en/product/ubx-m8030-series) is 
generally better, but also more costly, with faster time to fix and support more families of satellites.

The ham radio Raspberry need to keep track of the time to the nearest second, digital 
modes like FT8 rely on accurate timing. Ham radio also need your 
[Maidenhead](https://en.wikipedia.org/wiki/Maidenhead_Locator_System) grid cell 
location.

The script showpos.py emit the gpsd or SignalK position if it can find a suitable server
(it search the subnet for any gpsd or SignalK servers providing position).
It provide a quick way of both checking if your position and also if gpsd or SignalK works.
The script also launch a pop-up window displaying som vital information.
![Control window](https://github.com/olewsaa/amateur-radio/blob/main/on-the-move/showpos.png)

## GPSD
The gpsd config file is included in this repo,
[gpsd](https://github.com/olewsaa/Yacht-computer/blob/master/GPSD%20Position%20and%20Time/gpsd).

The config file assume that the GPS present itself as ```/dev/ttyACM0```, this is 
normally the case, but variants are known, ACM0 or USB0 etc. Check this.

As many other devices also present themselves as ttyACM? this might lead to 
conflicts. The CANbus to USB device and a few others also show up as ttyACM?,
this need to be sorted out, trial and error. The assigngment sequence seems
to be reproducable. More on this below.

Installing gpsd is straightforward 
```
apt install gpsd
```
and after starting the service it can 
be checked with a simple utility like gpsmon (it's part of the 
gpsd-clients package).
```
apt install gpsd-clients
```
After the GPS has found the satellites and gotten a fix it should report position 
and additional information, another utility is xgps which display a graphical 
map of the satellites.

To enable connection from clients in the network some settings need to be updated.
See the file gpsd in this repo for the gpsd config file, in addition the file
[gpsd.socket](https://github.com/olewsaa/Yacht-computer/blob/master/GPSD%20Position%20and%20Time/gpsd.socket) 
also need to be updated in order to allow for any client node in 
the network to connect. After a reboot any gpsd client (gpsmon, xgps etc) should be 
allowed to connect to the openplotter server.


### Device and port management
As there are several devices that show up as /dev/ttyACM[0,1,2...] management of these 
is  important. There is a comprehensive list of USB GPS  devices in the file with 
rules for user devices, /lib/udev/rules.d/60-gpsd.rules . If your device is found is this 
file it should show up as /dev/gps0 , if you have more then one or replugged recently 
/dev/gps1.

To check, use the following command to find the identity of your USB GPS device,
(asuming /dev/ttyACM0):
``` 
udevadm info --query=all --name=/dev/ttyACM0
``` 
Look for the fields for vendor id and model:
``` 
E: ID_VENDOR=u-blox_AG_-_www.u-blox.com
E: ID_VENDOR_ENC=u-blox\x20AG\x20-\x20www.u-blox.com
E: ID_VENDOR_ID=1546
E: ID_MODEL=u-blox_7_-_GPS_GNSS_Receiver
E: ID_MODEL_ENC=u-blox\x207\x20-\x20GPS\x2fGNSS\x20Receiver
E: ID_MODEL_ID=01a7
``` 
Then look for a match in /lib/udev/rules.d/60-gpsd.rules, if there is a match
then your device should show up at /dev/gps0 and you can use this in the gpsd condig file.

Example :
```
devadm info --query=all --name=/dev/ttyACM0 
E: ID_VENDOR=u-blox_AG_-_www.u-blox.com
E: ID_VENDOR_ENC=u-blox\x20AG\x20-\x20www.u-blox.com
E: ID_VENDOR_ID=1546
E: ID_MODEL=u-blox_7_-_GPS_GNSS_Receiver
E: ID_MODEL_ENC=u-blox\x207\x20-\x20GPS\x2fGNSS\x20Receiver
E: ID_MODEL_ID=01a7
```
```
grep 1546 /lib/udev/rules.d/60-gpsd.rules
ATTRS{idVendor}=="1546", ATTRS{idProduct}=="01a7", SYMLINK+="gps%n", TAG+="systemd", ENV{SYSTEMD_WANTS}="gpsdctl@%k.service"
```

If there is no match you might need to add a new entry with the relevant info. The 
two numbers needed are the ID '1546' and the model '01a7'.



## Chrony
 Chrony will provide time for it's own host are well as act a server for 
any other hosts that synchronise its clock to the yacht computer. 
Openplotter will try to set its time from SignalK if it can, but it recognise 
time daemon chrony and will step back if chrony is running. With an attached 
GPS chrony will always have time (when GPS has fix) and chrony should be in
control of the time. Installing chrony is simple just an apt install command.

There are two chrony config files in this repo, one for 
[client](https://github.com/olewsaa/Yacht-computer/blob/master/GPSD%20Position%20and%20Time/chrony.conf.client) 
and one for the  chrony 
[server](https://github.com/olewsaa/Yacht-computer/blob/master/GPSD%20Position%20and%20Time/chrony.conf.server). 

To check if things are working and chrony sync time via GPSD, try:
```
chrony tracking
```
or in loop:
```
watch chrony tracking
```


### Chrony control commands
If the timings are totally out of sync it might not step the clock. 
Hence setting the clock manual to get it right to the correct year, month, 
date and some time close to correct is needed.
```
chronyc manual on
chronyc settime May 27, 2023 10:35:00
```
Some often used commands include :
```
chronyc sources
chronyc sourcestats
chronyc tracking
```
The command 
```
chronyc help
```
will provide a nice list of commands that can be used.


### Position from GPDS
A Python script to ask the GPSD server for the position is need to update the config files
for ham radio, it can be done from SignalK when the chartplotter is working but also from GPSD
running on the Yacht computer. The simple Python GPSD CLI client is 
[gpsd.cliclient.py](https://github.com/olewsaa/Yacht-computer/blob/master/GPSD%20Position%20and%20Time/gpsd.cliclient.py)

