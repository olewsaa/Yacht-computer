#!/usr/bin/env python

# A simple Python script to test the SignalK server, e.g. that it will receive messages from 
# any source not only localhost.


import time, socket


# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# socket.AF_INET is  Internet
# socket.SOCK_DGRAM) is  UDP



print("Trying to send values to SignalK")

while True:
    t=input("Please enter a number: ")
    x = float(t)

    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"OrangePi.signalK.manual","value":'+format(x,'5.3f')+'}]}]}'
    print(SignalK)

    sock.sendto(SignalK, ('192.168.1.160', 55557))

