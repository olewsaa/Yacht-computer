#!/usr/bin/env python

import time, socket


# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# socket.AF_INET is  Internet
# socket.SOCK_DGRAM) is  UDP
# SignalK_UPD_PORT=55557
SignalK_UPD_PORT=20220


print("Trying to send values to SignalK")

while True:
    t=input("Please enter a number: ")
    x = float(t)

    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"OrangePi.signalK.manual","value":'+format(x,'5.3f')+'}]}]}'
    print(SignalK)

#    sock.sendto(SignalK, ('192.168.1.169', 55557))
    sock.sendto(SignalK, ('192.168.1.169', SignalK_UPD_PORT))

# SignalK :
# environment.wind.speedApparent
# environment.wind.angleApparent
