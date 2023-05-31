#!/usr/bin/env python3
#
# Usage:
# ./gpsd.cliclient.py  192.168.0.172 2947
# ./gpsd.cliclient.py 
# Latter will use defaults set in the code below.
#
# Written by Ole W. Saastad
# May 29. 2023
#
#
import sys, gps

host='192.168.0.172'
port='2947'

if len(sys.argv) == 2:
    host=sys.argv[1]
elif len(sys.argv) > 2:
    host=sys.argv[1]
    port=sys.argv[2]

#print(host,port)    
# Host and port is set directly :
#session = gps.gps(mode=gps.WATCH_ENABLE,port='2947',host='192.168.0.172')

session = gps.gps(mode=gps.WATCH_ENABLE, port=port, host=host)

#print(session)
for report in session:
    if report['class'] == 'TPV':
        print(report['lat'],report['lon'])
        lat=float(report['lat'])
        lon=float(report['lon'])
        if lon>0:
            l='E'
        else:
            l='W'
        print(format(lat,'9.5f')+' N',format(lon,'9.5f')+' '+l)
        print(format(int(lat),'03d')+'°',
              format((lat-int(lat))*60,'5.2f')+'\' N',)
        print(format(int(lon),'03d')+'°',
              format((lon-int(lon))*60,'5.2f')+'\'',l)
         
        break



    
