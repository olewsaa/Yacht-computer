
# Wifi 2 wifi gateway using long range USB antenna


## Wifi is desperately needed

While we are waiting for Starlink to become operational we have
limited acces to the internet on board while en-route. Hence the
demand for access is very high when getting to place where such luxury
is offered. Crew and skippers flock to the bars and cafes in the
marinas as free wifi is an excellent way to attact customers. It is
nice to have a long range antenna onboard to pick up these signals and
hence have wifi onboard.

### Wifi gateway

A wifi 2 wifi gateway using a Raspberry Pi and a long range USB high
gain antenna to provide local wifi coverage onboard is a must when
cruising. In addition it should be easy to use, any member of the crew
should ba able to select a shore station. They comes in all variants,
sometimes a code is posted and some times you'll need a reciept to get
the code etc etc the list is endless. On common fact is that they
generally do not offer connection below deck. Using a high gain
antenna outside to pick up the signal from a marina or a café and get
a reliable connection and then provide this connection to a local wifi
(cable also) segment onboard is a nice way of solving this
chellenge. While commercial solution do exsists (like Redbox), they
are generally expensive and bundled with a lot of other functions not
needed for this purpose.

## Changes in the default raspap-webgui setup

This setup is based on the raspap-webgui, https://raspap.com/ project.
Install according to instructions and when everything is set up and
working some changes can be applied. First of all the changes to use a
USB long range antenna as the WAN (Wide Area Network, aka the
"internet") is needed. This is described in the FAQ section. Using
wan1 as WAN connection. 

For most users who do not need internet access via the cable
connection this is all you need.

## Limiting the access to setup in the GUI

Users should only be allowed to scan for and change the shore station,
not mess up the settings. This is solved by selecting monitoring in
the config file. This setting is a FAQ entry in the raspiap pages.  It
works fine, users can log in with admin/pw and will only have access
to configure the client. 

## Changes to get eth0 working as Access point with dhcp

The files 
* /etc/dhcpcd.conf 
* /etc/dnsmasq.conf 
* /etc/network/interfaces 
* /var/www/html/includes/config.php
* /etc/hostapd/hostapd.conf 

need to be updated, only the extra lines
to be added are included in the files listed here. You cannot copy
these files, it will not work, you need to edit them. Remember to
change the MAC adresses to fit your system, use the command ifconfig
to list them.  After the changes have been applied a reboot is
needed. Things should now work with internet access available for
both cabled connected devices and wifi connected devices. 

To safeguard against unwanted updates I have marked all these files read-only.




