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

# Original code to request from Signal K - from user «Sailoog» at 
# openmarine forum.

import sys, json, requests

resp = requests.get('http://10.10.10.1:3000/signalk/v1/api/vessels/self/navigation/position/value', verify=False)
# Insert your local Signal K server name or IP number and default port 3000
if (resp.status_code == 404):
	exit(1)
# Just exit if no valid response from the SignalK server.

data = json.loads(resp.content)

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


# Main start here.

print(data['latitude'])
print(data['longitude'])

grid=to_grid(float(data['latitude']), float((data['longitude'])))
print(grid)

# end main

