# Config files for Raspberry Pi 


## Lost access point 
After an upgrade in May 2019, I lost the access point. Some search at various sites and forums was needed to resolve this. The actual fix was actually simple, just replacing a single line in a config file. 

### Script file for dnsmadq
The file /etc/init.d/dnsmasq had a line that needed to be replaced, the line 115
needed to be replaced. The actual file is presented in this directory. The original line is commented out and a working line is added.



