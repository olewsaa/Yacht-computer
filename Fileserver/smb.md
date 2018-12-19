# Sambe file server

Installing samba is straightforwared and well covered on the net.
```
smbpasswd  -a guest
```
The only thing to remember is that the samba user must have a linux user entry with the same 
name (not needed fo rsamba v. 4).

The USB disk or stick need to be mounted at boot time, remember to update the /etc/fstab.
To autostart samba at boot the following command must be issued.
systemctl enable samba 

