#!/usr/bin/env python3

# Function to get the position from the gpsd server.

def gpsdclient():
    import gps
    port='2947' # Port is default, seldom changed.
    host='localhost' # 10.10.10.1 for OpenPlotter.

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


# Main body start here.

lat, lon = gpsdclient()

SizeLat = 10
SizeLon = 20

LatL = round(lat) - SizeLat
LatH = round(lat) + SizeLat
LonL = round(lon) - SizeLon
LonH = round(lon) + SizeLon

# Lattitude :
HemL = 'N' if LatL >= 0 else 'S'
HemH = 'N' if LatH >= 0 else 'S'
# Longitude:
EWL = 'E' if LonL >= 0 else 'W'
EWH = 'E' if LonH >= 0 else 'W'



# ICON:50N,70N,10W,20E|1,1|0,24,48|WIND,MSLP    

print("{}{},{}{},{}{},{}{}".format(abs(LatL),HemL,abs(LatH),HemH,\
                                   abs(LonL),EWL,abs(LonH),EWH))

