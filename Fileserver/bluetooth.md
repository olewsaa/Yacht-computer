# Bluetooth setup

## Dsiribution used

[https://www.armbian.com/orange-pi-zero/#kernels-archive]
Running : Armbian_5.38_Orangepizero_Debian_stretch_next_4.14.14

## Instructions

Bluetooth server for sharing of files and photos

### Background web sites
[http://www.instructables.com/id/Bluetooth-How-to-send-a-copy-of-your-photos-and-vi/]
[http://www.instructables.com/id/Install-Bluetooth-in-Linux-System/]

### Step by step instructions

Step 1:
[http://www.instructables.com/id/Install-Bluetooth-in-Linux-System/]
```
apt-get update
 
apt-cache show bluez
service bluetooth start
service bluetooth status
bt-adapter -l
hciconfig -a
hcitool dev
hcitool scan

service dbus status
service bluetooth status
hciconfig -a
```
Step 2:
[http://www.instructables.com/id/Make-Raspberry-Pi-Discoverable-By-Remote-Bluetooth/]
```
hciconfig -a
hciconfig hci0 piscan
hciconfig -a hci0 | grep -i Name
```
Step 3:
[http://www.instructables.com/id/Bluetooth-How-to-send-a-copy-of-your-photos-and-vi/]
```
apt-get install obexpushd
ps aux | grep -i obexpushd ; kill processes.
```
Edit : /etc/systemd/system/dbus-org.bluez.service
add --compat
```
ExecStart=/usr/lib/bluetooth/bluetoothd --compat --noplugin=sap
```
From file: /etc/bluetooth/main.conf
```
# How long to stay in discoverable mode before going back to non-discoverable
# The value is in seconds. Default is 180, i.e. 3 minutes.
# 0 = disable timer, i.e. stay discoverable forever
DiscoverableTimeout = 0
```
Start the service
```
systemctl daemon-reload
systemctl restart bluetooth
obexpushd -B -n -o '/home/pi/storage' # This is for testing, use the service obexpushd
                                      # described in another file. 
chmod a+rwx /home/pi/storage/
chmod a+rx /home/pi
```
To automatically enable services after reboot :
```
systemctl enable  bluetooth
```
This should start the service at boot time.





Alternatives to do this, not tested by me :
https://askubuntu.com/questions/131570/how-do-you-make-ubuntu-accept-files-sent-
over-bluetooth


