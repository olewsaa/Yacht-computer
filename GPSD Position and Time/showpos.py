#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Python script to collect the current GPS position from 
# Signal K and display it.
#
# Adapted, collected and edited by Ole W. Saastad, LB4PJ
# 23 June 2022. 
# 14 July 2022 Added check for valid SignalK response  
# 08 August 2022 Changed to only displaying position
# 22 Decemer 2023 Added dd mm ss and dd mm.mmm printout

# Original code to request from Signal K - from user «Sailoog» at 
# openmarine forum.

import sys, math, json, requests

#resp = requests.get('http://10.10.10.1:3000/signalk/v1/api/vessels/self/navigation/position/value', verify=False)
# Insert your local Signal K server name or IP number and default port 3000
#if (resp.status_code == 404):
#	exit(1)
# Just exit if no valid response from the SignalK server.

#data = json.loads(resp.content)

# 
# Converting to grid, original code by Walter Underwood, K6WRU 
#
# Convert latitude and longitude to Maidenhead grid locators.
#
# Arguments are in signed decimal latitude and longitude. For example,
# the location of my QTH Palo Alto, CA is: 37.429167, -122.138056 or
# in degrees, minutes, and seconds: 37° 24' 49" N 122° 6' 26" W

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


#print(data['latitude'], data['longitude'])

def gpsdclient():
    import gps
    #host='192.168.0.107'
    #host='10.10.10.1'
    host='127.0.0.1'
    port='2947'
    session = gps.gps(host=host, port=port,mode=gps.WATCH_ENABLE)
    while 0 == session.read() :
        if not (gps.MODE_SET & session.valid):
            # not useful, probably not a TPV message
            continue        
        if ((gps.isfinite(session.fix.latitude) and
             gps.isfinite(session.fix.longitude))):
            print(" Lat %.6f Lon %.6f" %
                  (session.fix.latitude, session.fix.longitude))
            return session.fix.latitude, session.fix.longitude
            break
    session.close()
# End gpsdclient




# Main start here.

lat, lon = gpsdclient()
print(lat, lon)
#
print(math.floor(lat),'°',"%.3f" % (60*(lat-math.floor(lat))),"'")
print(math.floor(lon),'°',"%.3f" % (60*(lon-math.floor(lon))),"'")
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



# end main

