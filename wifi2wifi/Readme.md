
# A wifi 2 wifi gateway using a Raspberry Pi and a USB wifi

A wifi 2 wifi gateway using a Raspberry Pi and a USB wifi dongle or a
USB high gain antenna.  Written to provide wifi onboard a yacht when
wifi us only availale at a distance where only a high gain wifi
antenna with a USB connection. Only a good antenna can make a good connection
to the marina or a café. It does provide local in-numbers using a local dhcp and
network address translation (NAT) to forward the traffic to the external network
where the USB wifi is seen as a client. Hence the wifi2wifi label.

## Step by step instructions.

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

Edit the files, use the files in this repository as hints or just copy and replace the original. 
Remember to make a copy of the original ones, your files might not be identical. My distribution
is *Raspbian GNU/Linux 9*.
```
nano /etc/dhcpcd.conf
mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
nano /etc/dnsmasq.conf
nano /etc/hostapd/hostapd.conf
nano /etc/default/hostapd
nano /etc/sysctl.conf
```
copy in :
```
/etc/iptables.ipv4.nat
```
and
```
/etc/network/interfaces
```
Remember to replace MAC address with your Raspberry Pi internal wlan MAC address,
```
*hwaddress ether b8:27:eb:05:89:ec*.
```
Edit
```
nano /etc/rc.local
```
the extra lines I included. The dhcp service refuse to start when there are both a client and a server 
specifies in the /etc/network/interfaces.


Dhcp server do not start anyway, it finds wlan1 being a dhcp client, and fails du start. 
```
update-rc.d dhcpcd disable
```
Hence the dhcp start in rc.local



## Some extra commands can be handy:

Changing the IP tables at the comand line :
```
iptables -t nat -A POSTROUTING -o wlan1 -j MASQUERADE
iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i wlan0 -o wlan1 -j ACCEPT
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

### Clean up a lot of things not needed :
```
apt remove   bridge-utils  wpagui  claws-mail libreoffice wolfram wolfram-engine\
smartsim   openoffice.org   openoffice   libreoffice  minecraft-pi\
libreoffice-common libreoffice-core libreoffice-gtk libreoffice-gtk2 libreoffice-java-common\
libreoffice-sdbc-hsqldb libreoffice-style-galaxy libreoffice-systray 

apt autoremove
```




