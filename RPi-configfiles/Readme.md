# Config files for Raspberry Pi 

There are always some minor settings that can easily be overlooked. Most things comes nicely packaged with the [NOOB Openplotter distro](http://www.sailoog.com/blog-categories/openplotter-rpi).

## Forwarding
If laptop or other clients are connected to the OpenPlotter node requests to other destinations need to be forwarded, either to other 
nodes on the wifi range or via the Ethernet interface to a node which connect to the internet like a router (raspap using a RPi3).
Change the line in  /etc/sysctl.conf to read : net.ipv4.ip_forward = 1

## Lost access point 
As always, updates and upgrades are not always safe. Most major
disasters happen after an update or upgrade, where important services
are affected. After an upgrade in May 2019 I lost the access
point, all my IoToB devices failed to connect. 
Some search at various sites and forums was needed to resolve
this. The actual fix was actually simple, just replacing a single line
in a config file.

### Script file for dnsmadq
The file /etc/init.d/dnsmasq had a line that needed to be replaced, the line 115
needed to be replaced. The actual file is presented in this directory. The original 
line is commented out and a working line is added.

Some web references:

- [openmarine forum, thread about lost AP](http://forum.openmarine.net/showthread.php?tid=1083).

- [Some hints about AP config](https://forums.kali.org/showthread.php?38920-Access-point-configuration-problem-on-RPi3).

- [Failing to start dnsmasq](https://www.raspberrypi.org/forums/viewtopic.php?t=128449).

- ["dnsmasq not starting" at discourse](https://discourse.pi-hole.net/t/dnsmasq-not-starting/10523/13), containing pointer to the line that caused the problem.

- [Debian bug tracker](https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=858506), with the actual line. 

This last two references are the one that actually showed the error in the dnsmasq script (as always the devil is in the details).

### Config files for Access point and dhcp

As the Openplotter settings did not work as expected (Network mode:
RPi3: AP+client), hence I did the settings with config files. This is not the
proper way, but a hack needed to get things working. There are several
services working together and since I want an access point using the
wlan device and connect to the internet via an Ethernet cable the
settings is somewhat complex. In addition I do not want bridging from
the wlan to the internet. The wlan is a closed wireless net for the
Internet of Things on Board. The following is a list of files I
modified to get things to work, in addition the the script dnsmasq.
- /etc/dhcpcd.conf 
- /etc/hostapd/hostapd.conf
- /etc/dnsmasq.conf 

After modified these files I discovered that the access point was not enabled at reboot.
A systemd command to enable the service was issued ```systemctl enable  hostapd```. 
After this the access point were back and IoToB devices would work. 
