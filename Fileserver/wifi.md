# Wifi setup


Edit the interfaces:
```
nano /etc/network/interfaces
```
The file should look like :
```
source-directory /etc/network/interfaces.d
auto lo eth0
allow-hotplug eth0
iface lo inet loopback
iface eth0 inet dhcp

auto wlan0
iface wlan0 inet dhcp
wpa-ssid TeamRocketHQ
wpa-psk password
```

Change SSID and password to fit your needs.
