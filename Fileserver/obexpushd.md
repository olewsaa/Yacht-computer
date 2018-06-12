nano /etc/systemd/system/obexpush.service

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


systemctl enable obexpush
service obexpushd start



cat /proc/partitions 
nano /etc/fstab
mount -a
df -h
ls /storage/

