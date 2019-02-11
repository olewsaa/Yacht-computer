
# A wifi 2 wifi gateway using a Raspberry Pi and a USB wifi

A wifi 2 wifi gateway using a Raspberry Pi and a USB wifi dongle or a
USB high gain antenna.  Written to provide wifi onboard a yacht when
wifi us only availale at a distance where only a high gain wifi
antenna will get acceptable connection. Quite often only a good antenna
can make a good connection to the marina or a café. It does provide
local ip-numbers (192.168.1.xx) using a local dhcp and network address
translation (NAT) to forward the traffic to and from the external network
where the USB wifi is seen as a client. Hence the wifi2wifi label.

In addition it turned out to be useful to have a cabled network also connected 
to the gateway. The OpenPlotter RPi is in addtion to be a plotter and a SignalK 
server also a nice all purpose computer with a 19" screen, so having internet access 
is very nice. Not to mention the ease of doing charts software updates directly. 
I used a different ip range for the cabled network, mostly to keep them separated.

This project have a [project page](https://sites.google.com/site/olewsaa/yacht-server/raspberry-pi-as-a-router-gateway)

## Step by step instructions.

It should not be too hard to set up a Raspberry Pi to become a wifi
to wifi gateway.
My distribution is *Raspbian GNU/Linux 9*, *Linux raspberrypi 4.14.79-v7+ #1159*
as of December 2018.
For images and information about using [displays for RPi](https://www.waveshare.com/wiki/3.5inch_RPi_LCD_(A)).

I have used nano as an editor, since emacs (the king of editors) will take up too much 
space on the limited micro-sd card, more on cleaning up unwanted packages below. 

Just replacing the files provided should work, but make a backup of your files before
replacing them with the provided ones. It's only tested on this distribution.

### Clean up and start :
```
apt update
apt upgrade
apt autoremove 
reboot
apt install dnsmasq hostapd
```
Stop services while we are updating.
```
systemctl stop hostapd
systemctl stop dnsmasq
```

Edit the files below, use the files in this repository as hints or just
copy and replace the original. Remember to make a copy of the original
ones, your files might not be identical.

```
nano /etc/dhcpcd.conf
mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
nano /etc/dnsmasq.conf
mkdir -p /etc/hostapd/
nano /etc/hostapd/hostapd.conf
nano /etc/default/hostapd
```
Only the line with net.ipv4.ip_forward=1 need updating, removing the comment mark. 
```
nano /etc/sysctl.conf
```
copy in :
```
/etc/iptables.ipv4.nat
```
If you want to prepare this youself the commands are :
```
iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE
iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i wlan0 -o wlan1 -j ACCEPT
sh -c "iptables-save > /etc/iptables.ipv4.nat"
```
Next step is to update the interfaces file. It is in this file where wlan0 and wlan1
are described, wlan0 is the internal wifi interaface (server, dhcp server local) and
wlan1 is the external wifi client which obtain an ip address (dhcp client request)
from the hotspot wifi provider ashore. It was not needed to generate exra iptables and
routing entries for the eth0 interface. The routing command show that it forward to 
wlan1 without extra settings. 
```
nano /etc/network/interfaces
```
Remember to replace MAC address with your Raspberry Pi internal wlan MAC address,
```
iface wlan0 inet manual
hwaddress ether b8:27:eb:05:89:ec.
```
The address above is for the wlan0 (internal wifi device), but an additional line
is needed for the eth0 interface, like:
```
iface eth0 inet manual
hwaddress ether b8:27:eb:50:dc:b9
```

Edit
```
nano /etc/rc.local
```
the extra lines I included. The dhcp service refuse to start when there are both a client and a server 
specifies in the /etc/network/interfaces.


Dhcp server do not start anyway, it finds wlan1 being a dhcp client, and fails du start.
There is a test in the startup script. 
```
update-rc.d dhcpcd disable
```
Hence the manual dhcp start in rc.local, this will not restart automatically  if the
dhcp server fails or stops for some reason. It will need to be restared manually.


## Some extra commands that can be handy:

Changing the IP tables at the comand line (eth0 seems to automatically 
covered by the first line) :
```
iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE
iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i wlan0 -o wlan1 -j ACCEPT
```
Copy the setup to a file and reading the setup from a file: 
```
sh -c "iptables-save > /etc/iptables.ipv4.nat"
iptables-restore < /etc/iptables.ipv4.nat
```
### Routing need to be fixed and here are some commands that can be useful.
```
route del -net 0.0.0.0 gw 192.168.1.1 netmask 0.0.0.0 dev eth0
route del -net 192.168.1.0  gw 0.0.0.0 netmask 255.255.255.0 dev eth0
route del -net 192.168.0.0  gw 0.0.0.0 netmask 255.255.255.0 dev wlan0
route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         192.168.1.1     0.0.0.0         UG    304    0        0 wlan1
192.168.1.0     0.0.0.0         255.255.255.0   U     304    0        0 wlan1
```
Test the route with a ping command like :
```
ping vg.no
```

### Connecting using name
The box is given the name greybox and using ssh pi@greybox works fine when conecting 
via the wireless network. If you install a web server like:
```
apt-get install apache2 -y
```
then a device connected to the wireless network provided can connect the box using a 
name only, in my case greybox, http://greybox . This is enough to open up a default 
web page. 

(I chose the name greybox, as it cannot be compared to redbox, but try to do a few
of the things redbox does.)

I stared to look into the commands needed to do this at the command line, the script 
scan.py is a simple start.


### Clean up a lot of things not needed :
This list might be slightly longer than whars installed default, it might be that some 
packaged are not installed and hence cannot be removed, which will trigger warnings.
```
apt remove bridge-utils  wpagui  claws-mail libreoffice wolfram wolfram-engine\
smartsim  openoffice.org  openoffice  libreoffice  minecraft-pi\
libreoffice-common libreoffice-core libreoffice-gtk libreoffice-gtk2 libreoffice-java-common\
libreoffice-sdbc-hsqldb libreoffice-style-galaxy libreoffice-systray 

apt autoremove
```




