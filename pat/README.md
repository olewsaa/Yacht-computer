# Pat hint and tips

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


## Installation hints

### pat
Download pat, check [pat releases](https://github.com/la5nta/pat/releases),
below is shown 0.12.1 version, this gets updated regularly.

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


## ARDOP

The Amateur Radio Digital Open Protocol [ARPAP](https://www.cantab.net/users/john.wiseman/Documents/ARDOPC.html) is used as the transport layer.

The ardopc is distributed as a binary, source code is available.

     sudo -i
     cd /root
     mkdir ardop ; cd ardop
     wget https://www.cantab.net/users/john.wiseman/Downloads/Beta/piardopc
     cp piardopc /usr/local/bin/`

This would install the binary and make it accessable by default.

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
	  

## Hamlib

Become root and install hamlib.

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

Back as your favorite user,

     rigctl -l| grep -i flrig

*4  FLRig  FLRig  202101014.0  Stable  RIG_MODEL_FLRIG*
Entry number 4, stable support.

Hamlib support flrig and you can control your rig via flrig.

Test:

     rigctl -m 4 f
     rigctl -m 4 M USB 2700
     rigctl -m 4 M LSB 2400
     rigctl -m 4 T 1
     rigctl -m 4 T 0`

Documentation for [Hamlib](https://github.com/Hamlib/Hamlib/wiki/Documentation)

Pat need to known your rig control,
Edit .config/pat/config.json unser sections *hamlib rigs*, 
*ax25* and *ardop*.

     "hamlib_rigs": {"G90": {"address": "localhost:4532", "network": "tcp"}}

     "ax25" section, "rig": "G90"
     "ardop" section, "rig": "G90"



## Running

https://github.com/la5nta/pat/wiki/The-web-GUI

I have opted for manual control as I don't want the
PAT email setup to be active at all time. I have the RPi4 
installed as a yacht computer with openplotter and CAN bus interface,
with signalK dashboard etc. 


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
Since the computer is running a lot of services related to OpenPlotter and other
navigational stuff I do not want to have the Pat relater services running all the time.
I turn them on when needed and off when no longer used. To do this I have 
written a GUI with start, stop and PID buttons, the latter to check that the
services have started. 

It's a combination of Python GUI and bash scripts. 

The script is placed on my desktop with a "Postman Pat" icon (copyrighted ?). 

If you have a dedicated RPi there is no need, just launch the servives at boot time. 
This is nicely explained in the videos.

I'm using a Xiegu G90 for which Hamlib (current version March 2022) don't support.
Howver, flrig support G90 and I'm using hamlib with device 4 which is flrig. 
I still need to press the "Tune" button after changing frequency as my hamlib build
do not support this function. 
