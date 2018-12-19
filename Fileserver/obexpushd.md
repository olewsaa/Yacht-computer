# Bluetooth service

## Instructions

Edit the file :
nano /etc/systemd/system/obexpush.service
to include lines below.
```
[Unit]
Description=OBEX Push service
After=bluetooth.service
Requires=bluetooth.service

[Service]
ExecStart=/usr/bin/obexpushd -B -n -o /storage 

[Install]
WantedBy=multi-user.target
```
nano /lib/systemd/system/obexpushd.service

Same file as above, cp is also ok.

Then start the services:
```
systemctl enable obexpush
service obexpushd start
```

Set up storage. See also the file bluetooth.md.
```
cat /proc/partitions 
nano /etc/fstab
mount -a
df -h
ls /storage/
```
