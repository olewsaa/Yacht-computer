#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Python script to collect the current GPS position from 
# gpsd or Signal K and display it.
#
# Adapted, collected and edited by Ole W. Saastad, LB4PJ
# 23 June 2022. 
# 14 July 2022 Added check for valid SignalK response  
# 08 August 2022 Changed to only displaying position
# 22 Decemer 2023 Added dd mm ss and dd mm.mmm printout
# 10 January 2024 Added popup window with data
# 12 January 2024 Added check for gpsd and SignalK sources. 
#
# Original code to request from Signal K - from user «Sailoog» at 
# openmarine forum.
#
# In order for hostname and ipnumber to work correctly and
# provide a subnet to search for gpsd and signalk server
# the til /etc/hosts need to contain a line with hostname
# and ip number.
# Like :
# 127.0.0.1	localhost
# 10.10.10.1	openplotter
# 192.168.1.160	SSB
# 

# Enter known servers here to test these before starting to
# scan the whole subnet which takes some time.

known_servers=['127.0.0.1', '127.0.1.1', '192.168.0.160', '10.10.10.1']

# Add any others possible servers in addition to the ones listed.
# IP numbers are text strings.


import sys, math, json, requests
import tkinter as tk


# Definition of functions:

# This check the subnet for gpsd and signalk servers.
# It check the ones listed in known_servers first, if this
# is unciccessful it does a scan over all ip addresses.
# This can take some time.
#
def check_sources():
    import socket
    # Scan for GPSD service:
    def scan_for_gpsd(host, port):
        try:
            # Create a socket object
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # Set a timeout to handle non-responsive hosts quickly
            s.settimeout(1)
            # Attempt to connect to the host and port
            s.connect((host, port))
            # If successful, print a message
            print(f"GPSD service found on {host}:{port}")
            return host
        except (socket.timeout, socket.error):
            # Handle timeout or connection error
            #print(f"No GPSD service found on {host}:{port}")
            s.close()
            return '0.0.0.0'
        # Close the socket
        s.close()
        return '0.0.0.0'
        return

    def scan_for_signalk(host):
        import requests
        #print("in scan for signalk ",host)
        try:
            requeststring='https://'+host+':/signalk/v1/api/vessels/self/navigation/position/value'
            resp = requests.get(requeststring, verify=False, timeout=2)
            #print(resp.status)
            if (response.status_code == 200):
                print("SignalK server found",host)
                return host            
            if (resp.status_code == 404):return  'demo.signalk.org'
        except requests.exceptions.Timeout:
            print("Timeout",host)
            return '0.0.0.0'
        finally:
            return '0.0.0.0'

# Function check sources block start here

    hostname = socket.gethostname()
    #print(hostname)
    ip_address = socket.gethostbyname(hostname)
    #print(ip_address)
    subnet='.'.join(ip_address.split('.')[:-1])+'.'
    
    #print("IP subnet: ", subnet)
    #subnet = '192.168.0.'    
    
    #print("Scan ",subnet+'*'," for GPSD server")
    # Set the common GPSD port (default is 2947)
    gpsd_port = 2947

    # Scan a range of IP addresses for GPSD server, but check the known first.

    servers_to_test=known_servers
    #print(known_servers)
    for i in range(1, 255):
        servers_to_test.append(f"{subnet}{i}")

    for host in servers_to_test:
        #print("Host : ", host)
        host=scan_for_gpsd(host, gpsd_port)
        if host != '0.0.0.0' :
            #print("GPS server found", host)
            return host, 'gpsd'

    print("Scan for SignalK server")    
    for host in servers_to_test:
        #print("Host : ", host)    
        host=scan_for_signalk(host)
        if host != '0.0.0.0' :
            print("SignalK server found", host)
            return host, 'signalk'
        else:
            return 'demo.signalk.org', 'signalk'
# End check sources



# Function to convert from lat, long to Maidenhead GRID.

upper = 'ABCDEFGHIJKLMNOPQRSTUVWX'
lower = 'abcdefghijklmnopqrstuvwx'

def to_grid(dec_lat, dec_lon):
    if not (-180<=dec_lon<180):
        sys.stderr.write('longitude must be -180<=lon<180, given %f\n'%dec_lon)
        sys.exit(32)
    if not (-90<=dec_lat<90):
        sys.stderr.write('latitude must be -90<=lat<90, given %f\n'%dec_lat)
        sys.exit(33) # can't handle north pole, sorry, [A-R]

    adj_lat = dec_lat + 90.0
    adj_lon = dec_lon + 180.0

    grid_lat_sq = upper[int(adj_lat/10)];
    grid_lon_sq = upper[int(adj_lon/20)];

    grid_lat_field = str(int(adj_lat%10))
    grid_lon_field = str(int((adj_lon/2)%10))

    adj_lat_remainder = (adj_lat - int(adj_lat)) * 60
    adj_lon_remainder = ((adj_lon) - int(adj_lon/2)*2) * 60

    grid_lat_subsq = lower[int(adj_lat_remainder/2.5)]
    grid_lon_subsq = lower[int(adj_lon_remainder/5)]

    return grid_lon_sq + grid_lat_sq + grid_lon_field + grid_lat_field + grid_lon_subsq + grid_lat_subsq


# Function to retrieve the GPS position from the SignalK server.

def signalkclient():
    import json, requests, urllib3
    
#    print("SignalK client host",host)
    urllib3.disable_warnings()  # Disable the warnings.    
    
    requeststring='https://'+host+':/signalk/v1/api/vessels/self/navigation/position/value'
    resp = requests.get(requeststring, verify=False) 
    
    if (resp.status_code == 404):
        return 0.0, 0.0
        # Just return zeros if no valid response from the SignalK server.

    data = json.loads(resp.content)
    return float(data['latitude']), float(data['longitude'])
# End signalkclient    


# Function to get the position from the gpsd server.

def gpsdclient():
    import gps
    port='2947' # Port is default, seldom changed.

    session = gps.gps(host=host, port=port,mode=gps.WATCH_ENABLE)
    while 0 == session.read() :
        if not (gps.MODE_SET & session.valid):
            # not useful, probably not a TPV message
            continue        
        if ((gps.isfinite(session.fix.latitude) and
             gps.isfinite(session.fix.longitude))):
#            print(" Lat %.6f Lon %.6f" %
#                  (session.fix.latitude, session.fix.longitude))
            return session.fix.latitude, session.fix.longitude
            break
    session.close()
# End gpsdclient


# Function to display the server and position in a few formats. 

def display_info(serv, lat, lon, latd, lond, grid):
    tk.Label(master, text="Position",anchor="e").grid(row=0,columnspan = 2)
    lb=["Server   ","Lat    ","Lon    ","Lat    ","Lon    ","Grid   "]
    a=[serv, lat, lon, latd, lond, grid]
    for r in range(len(lb)):
        tk.Label(master, text=lb[r]).grid(row=r+1)
        e  = tk.Entry(master, width="14")
        e.insert(0, a[r])
        e.grid(row=r+1, column=1)




# Main routine  start here.


host, type = check_sources()
print("Checking sources found : ", host, type)
if type == 'gpsd':
    lat, lon = gpsdclient()
elif type == 'signalk':
    lat, lon = signalkclient()
else:
    print("No position servers found, exiting")
    exit(1)
    
print(lat, lon)
#
print(math.floor(lat),'°',"%.3f" % (60*(lat-math.floor(lat))),"'")
print(math.floor(lon),'°',"%.3f" % (60*(lon-math.floor(lon))),"'")
#
latd = "N "+str(math.floor(lat))+'°'+str("%.3f" % (60*(lat-math.floor(lat))))+"'"
lond = "E "+str(math.floor(lon))+'°'+str("%.3f" % (60*(lon-math.floor(lon))))+"'"
#
min, sec = divmod(abs(lat)*3600, 60)
deg, min = divmod(min, 60)
print(math.floor(deg),'°',math.floor(min),"'","%.0f" % round(sec,0),'"')
min, sec = divmod(abs(lon)*3600, 60)
deg, min = divmod(min, 60)
print(math.floor(deg),'°',math.floor(min),"'","%.0f" % round(sec,0),'"')

#print(data['latitude'])
#print(data['longitude'])

#grid=to_grid(float(data['latitude']), float((data['longitude'])))
grid=to_grid(lat, lon)

print(grid)


# Make a GUI pop-up with dd.ddddd and dd mm.mmm positions.

master = tk.Tk()
master.title("Position")
resp="OK"

# Show the position as a popup X11 window.
display_info(host, lat, lon, latd, lond, grid)
tk.Button(master, text=resp, height="2",
          command=master.quit).grid(row=7, column=1, sticky=tk.W, pady=1)
master.mainloop()


# end main

