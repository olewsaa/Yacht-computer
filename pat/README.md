

# Pat hint and tips

  * [Introduction](#introduction)
  * [Installation hints](#installation-hints)
    + [Pat](#Pat)
    + [ARDOP](#ARDOP)
    + [VARA](#VARA)
    + [Hamlib](#Hamlib)
  * [Running](#Running)
  * [Script to control PAT](#script-to-control-pat)
  * [Grid coord. from Signal K](#Grid-from-Signal-K)
  * [Position from OpenPlotter](#Position-from-OpenPlotter)


## Introduction 
Using pat with Raspberry Pi4.

Please review the instructions found at [getpat](https://getpat.io/)
and possibly the 3 video instructions at

- Installing pat [Part 1](https://youtu.be/oZJJLfVz23k) 
- Installing pat [Part 2](https://youtu.be/BgqsTbxzR4E) 
- Installing pat [Part 3](https://youtu.be/ZCMHeuNjhK0)

Below is provided a quick cut and paste installation example.

We assume the rig control flrig is installed. I use a Xiegu G90
which is supported in version 1.4.5. See [fldigi](http://www.w1hkj.com/),
[flrig download](http://www.w1hkj.com/files/flrig/).

This [page](https://f1rum.fr/posts/mails-via-hf-with_ardop-and-pat/) provide 
a nice graphic explanation with a [figure](https://f1rum.fr/img/pat-archi.png) 
showing control and data flow, including port numbers.

The different script used to start the servies are in two classes, one without VARA
where the pat start script start everything including ARDOP. While the other script 
start and stop pat but not VARA as this har its own icon. This is simple start and 
stop, if you want something more sophisticated try 
[pat menu](https://github.com/km4ack/patmenu2).

As gpsd interface is not yet fully functional the script update.pos.pat.conf.py will 
alter the pat configure file and update the current position som the SignalK server.
If you run a stand alone RPi with no internet connection, you can update the time 
and date from the SignalK server using the set-date-from-SignalK.py script.


## Installation hints

### Pat
Download Pat, check [pat releases](https://github.com/la5nta/pat/releases),
below is shown 0.12.1 version, this gets updated regularly. This version do not
support VARA, if you want to use VARA see the VARA section below. You need a special 
version of pat to use VARA. Install this special version and not the version given here.
The dpkg command is the same, it's just another pat*deb package file. 

     sudo -i 
     wget https://github.com/la5nta/pat/releases/download/v0.12.1/pat_0.12.1_linux_armhf.deb
     dpkg --install pat_0.12.1_linux_armhf.deb`

As your prefered user, in my case just pi.

*pat configure*

change some variables (as the youtube suggest I use port 5000 for pat),

      "mycall": "Callsign",
      "secure_login_password": "PASSWORD",
      "auxiliary_addresses": [],
      "locator": "JO59JX",
      "service_codes": [
        "PUBLIC"
      ],
      "http_addr": "0.0.0.0:5000",
	  
save and exit. 
If you do not have winlink account contacting them for the first time will
issue a password for you, edit the file : $HOME/.config/pat/config.json 

You can find copy of my [config.json](https://github.com/olewsaa/Yacht-computer/edit/master/pat/config.json).
More on the [web GUI](https://github.com/la5nta/pat/wiki/The-web-GUI).

### ARDOP

The Amateur Radio Digital Open Protocol [ARPAP](https://www.cantab.net/users/john.wiseman/Documents/ARDOPC.html) is used as the transport layer.

The ardopc is distributed as a binary, source code is available.

     sudo -i
     cd /root
     mkdir ardop ; cd ardop
     wget https://www.cantab.net/users/john.wiseman/Downloads/Beta/piardopc
     cp piardopc /usr/local/bin/`

This would install the binary and make it accessable by default.

To install the ARDOP GUI

	 sudo -i
     cd /root
     cd ardop
     wget http://www.cantab.net/users/john.wiseman/Downloads/Beta/piARDOP_GUI
     cp  /usr/local/bin/`


Test for USB soundcard / audio device :

     arecord -l

record the device 
(I got: card 2: Device (USB Audio Device), device 0: USB Audio (USB Audio))

List the audio devices / soundcards

     cat   /proc/asound/card
     cat   /proc/asound/card3/stream0
	 
The relevant one yield:
Rates: 8000, 16000, 24000, 32000, 44100, 48000, Bits 16.

Start piardopc (use port 8515, btw is the default), 2 for card number and 0 for device 0.

     ./piardopc 8515 plughw:2,0 plughw:2,0


You can try to set the ARDOP speed to 2000, as your user do

     nano .config/pat/config.json

     "ardop": {
     "addr": "localhost:8515",
     "arq_bandwidth": {
       "Forced": false,
       "Max": 2000`

### VARA

First step to use VARA with pat is to download and install a special VARA enabled 
pat version. This is located on a google drive by Rainer Großkopf and Chris Keller
(two names shows up on updates, unsure who to credit) :
https://drive.google.com/drive/folders/1v__JXBHHD7w11SS5WyfPtMmVrzZ-uqx3

The latest version (June 2022) is : pat_45c5064_linux_armhf.deb , this is the ARM 32 bit 
version who work well with my RPi4. Close any running pat and install this package. 
After restart of pat there should be a VARA entry in the menu of possible connections
in the web interface. 

Using VARA on a Raspberry Pi is not simple, the VARA is Windows
program compiled for x86 processors. This require the Box86 x86
emulator and the Windows emulator Wine. While Wine doesn't requite
much extra performance the emulator doesn't. It runs nicely if a
dedicated RPi4 is used, but using a RPi 4 that run all the services of
OpenPlotter VARA fails.

The installation is relatively straightforward. Good instructions can be found, 
https://github.com/WheezyE/Winelink/blob/main/docs/README.md
I suggest doing it a bit more manual, as it's a one time job.

      wget https://raw.githubusercontent.com/WheezyE/Winelink/main/install_winelink.sh
      chmod +x install_winelink.sh 
      ./install_winelink.sh vara_only

It takes some time and the installer use X11 (Wine) so you need a
console VNC or screen. It does not work using a ssh connection (maybe
if you set up X11 forwarding? - I did not try). You need to answer a
few questions. Select your audio device, often a USB device.

The instructions found in the git repo linked to above give far more details. 

If you want full VARA support you need to email the author and forward the
license fee. My experience is very good, VARA works as good as the
user stories tell. Two main issues with VARA is Windows/x86 and the
license.

Keep a copy of the script as it deletes itself when run, saves you from downloading it again if something goes wrong (as it some times do).

The pat config file need to have a vara section, I use the following:

 	"vara": {
   	"host": "localhost",
   	"cmdPort": 8300,
   	"dataPort": 8301,
   	"rig": "G90",
   	"ptt_ctrl": true
   	},

### Hamlib

Become root and install hamlib. Hamlib do not support Xiegu G90 in the 4.4 version.

     sudo -i
     mkdir hamlib; cd hamlib
     https://github.com/Hamlib/Hamlib/releases/download/4.4/hamlib-4.4.tar.gz

Building,

     tar xf hamlib-4.4.tar.gz
     cd hamlib-4.4
     ./configure
     make
     make install
     ldconfig`

Back as your favourite user,

     rigctl -l| grep -i flrig

*4  FLRig  FLRig  202101014.0  Stable  RIG_MODEL_FLRIG*
Entry number 4, stable support.

Hamlib support flrig and you can control your rig via flrig.

Test, run interactively from the shell:

     rigctl -m 4 f
     rigctl -m 4 M USB 2700
     rigctl -m 4 M LSB 2400
     rigctl -m 4 T 1
     rigctl -m 4 T 0`

Documentation for [Hamlib](https://github.com/Hamlib/Hamlib/wiki/Documentation)

Pat need to known your rig control,
Edit .config/pat/config.json under sections *hamlib rigs*, 
*ax25*, *ardop* and *VARA*

     "hamlib_rigs": {"G90": {"address": "localhost:4532", "network": "tcp"}}

     "ax25"  section, "rig": "G90"
     "ardop" section, "rig": "G90"
     "vara"  section, "rig": "G90"


## Running

https://github.com/la5nta/pat/wiki/The-web-GUI

I have opted for manual control as I don't want the
PAT email setup to be active at all time. I have the RPi4 
installed as a yacht computer with openplotter and CAN bus interface,
with Signal K dashboard etc. 


As user :
flrig must be started first. More extensive
scripts are found together with this file.

Here are some simple examples:

Starting deamon for flrig, 4 is flrig.

     rigctld -m 4 > rigctld.log & 

Starting ardop

     piardopc 8515 plughw:2,0 plughw:2,0 > piardopc.log & 
	 
Start pat

     pat http > pat.log &



Setting frequency works nice from PAT. 
While rigctl works nicely:

     rigctl -m 4 T 1
     rigcrl -m 4 T 0

An update of setting is needed in the config file to use PTT.

Enable PTT here :

    "rig": "G90",
       "ptt_ctrl": true,
       "beacon_interval": 0,
       "cwid_enabled": true`

The alternative to rig control is using VOX.


## Script to control PAT
Since the computer is running a lot of services related to OpenPlotter
and other navigational stuff I do not want to have the Pat relater
services running all the time.  I turn them on when needed and off
when no longer used. To do this I have written a GUI with start, stop
and PID buttons, the latter to check that the services have started.

As touched upon the introduction,
there are two set of scrips, one set for the control when only ARDOP is 
used and another when VARA is used, I start and stop VARA with its icon,
and use the pat.start to launch just pat and if necessary flrig and hamlib.
As said in the beginning, if you run only ARDOP it can run on the same 
Raspberry as the OpenPlotter services. 

The script pat-control.py is used to launch and test the services for
a pure ARDOP setup. Maybe this should be cleaned up :)
It's a combination of Python GUI and bash scripts. 

The script is placed on my desktop with a "Postman Pat" icon (copyrighted ?). 

If you have a dedicated RPi and want pat active at all times there is no need for 
any of these scripts, just launch the services at boot time.  This is nicely 
explained in the videos. I use the rig to receive weatherfax, RTTY forecasts and
record MSI audio messages all which need the audio device. Hence ARDOP and VARA 
should be stopped. Pat could probaly be left running, but I stop that too when 
not doing email communication. 

I'm using a Xiegu G90 for which Hamlib (current version March 2022)
don't support.  However, flrig support G90 and I'm using hamlib with
device 4 which is flrig.  I still need to press the "Tune" button
after changing frequency as my hamlib build do not support this
function.


## Grid from Signal K
The code get.grid.py is a Python script to request a position from
the Signal K server and convert it to a Grid representation to be 
used for the current position. AFAIK pat will support gpsd, but I
would be nice to support Signal K also. In this case the position 
would be updated automatically when a position report is composed.

The script update.pos.pat.conf.py will request a position from the
Signal K server and update the pat config file. Runing this before 
launching pat your current position will be used. As you sail along
towards your destination your position changes and you need to restart pat
to get the current position. 


## Position from OpenPlotter
Pat has an interface to gpsd to obtain position from GPS when composing
a position report. When no GPS is attached on USB this becomes a bit more
complicated as gpsd expect more than just the position. 

OpenPlotter is a hub on board and it accept GPS from both AIS transceiver
and chart plotter. The chart plotter GPS provide all needed data to fulfill 
gpsd's needs, it want a bit more data than just time/lat/long. Data is exported
over tcp on a dedicated port (default 10110) as NMEA183 sentences which gpsd happily accept.

Editing the gpsd config file at /etc/defaults/gpsd (in this repo) will enable gpsd to receive 
it's GPS data from OpenPlotter instead of a USB GPS. OpenPlotter must be configured
to export these NMEA183 sentences. 

Pat's config need to be edited to use gpsd to get the current position:
```
 "gpsd": {
        "enable_http": true,
        "allow_forms": true,
        "use_server_time": false,
        "addr": "localhost:2947"
    },
```
An interface for SignalK would be more elegant. 

