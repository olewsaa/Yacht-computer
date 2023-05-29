# Position and Time

## Introduction
Position and time are always of interest. There are several sources available
onboard:
1. The internet provide time from ntp servers if connected to the net.
2. The Chart plotter (when turned on) provide both time and position via NMEA2000, SignalK and OpenPlotter.
3. Then no other source is available an attached USB GPS will provide the Yacht computer with both time and position.

As one need to save power when at anchor the sailing instruments and chart 
plotter are turned off. The yacht computer is very handy to have on as it 
record air pressure. Wind require the instruments to be on, but not the 
chart plotter. The simple GPS "mouse" will provide a time nice time signal
to the yacht computer which in turn provide time to any other computer 
(the ham radio one) and any laptops.  The ham radio Raspberry need to keep
track of the tme to the nearest second. Digital modes like FT8 rely on 
accurate time.

## GPSD
The gpsd config file is included in this repo,
[https://github.com/olewsaa/Yacht-computer/blob/master/Pos_and_Time/gpsd](gpsd).

The config file assume that the GPS present itself as /dev/ttyACM0, this is 
normally the case, but variants are known, ACM0 or USB0 etc. Check this.

As many other devices also present themselves as ttyACM? this might lead to 
conflicts. The CANbus to USB device and a few others also show up as ttyACM?,
this need to be sorted out, trial and error. The assigngment sequence seems
to be reproducable. It's possible to bind each device to a specific USB port,
but this is on the to-do list.

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
gpsd.socket (also in this repo) also need to be updated in order to allow for any client node in 
the network to connect. After a reboot any gpsd client (gpsmon, xgps etc) should be allowed to
connect to the openplotter server.

## Chrony
Chrony will provide time for it's own host are well as act a server for 
any other hosts that synchronise its clock to the yacht computer. 
Openplotter will try to set its time from SignalK if it can, but it recognise 
time daemon chrony and will step back if chrony is running. With an attached 
GPS chrony will always have time (when GPS has fix) and chrony should be in
control of the time. Installing chrony is simple just an apt install command.

There are two chrony config files in this repo, one for client and one for the 
chrony server. 

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

