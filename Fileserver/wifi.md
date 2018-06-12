Edit /etc/network/interfaces

cat /etc/network/interfaces
  #interfaces(5) file used by ifup(8) and ifdown(8)
  #Include files from /etc/network/interfaces.d:
source-directory /etc/network/interfaces.d
auto lo eth0
allow-hotplug eth0
iface lo inet loopback
iface eth0 inet dhcp

auto wlan0
iface wlan0 inet dhcp
wpa-ssid TeamRocketHQ
wpa-psk password

