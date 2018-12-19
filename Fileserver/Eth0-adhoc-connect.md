# Ethernet setup

## Ad hoc setup for cable ethernet
When encounter a new wifi network where SSID and password need to be entered into the OrangePi 
server a cable can be used between the laptop (which provide both SSID and password, as we need 
to enter it into the OragePi). 

Just connect the units with a RJ45 cable and select "Shared with other computers" instead of the 
more common "Automatic (DHCP)".  After a few seconds the ifconfig command will show an ipaddress 
on the eth0, ifconfig eth0. 
```
service restart network
```
and the orange pi should now be on the wifi net with as assigned ip from the DHCP server. You can 
use the arp -a command to find it, or log in via the RJ45 cable from the laptop.

This is the laptop's eth0 ad-hoc network address. To find the OrangePi units's address we need to 
issue an arp comand, arc -a. It will whow an ip address associated with eth0.

Then it's simple matter of logging in to the OrangePi using ssh. 

Use ssh ip-address to log in as root or orangepi user and edit the 
config files, 
```
nano /etc/network/interfaces
```
and update the SSID and password as needed. 

