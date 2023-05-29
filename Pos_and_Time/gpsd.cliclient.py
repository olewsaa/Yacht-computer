#! /usr/bin/env python3
import gps

# Host and port is set directly :
session = gps.gps(mode=gps.WATCH_ENABLE,port='2947',host='192.168.0.172')

print(session)
for report in session:
    if report['class'] == 'TPV':
        print(report['lat'])
        print(report['lon'])
        break



    
