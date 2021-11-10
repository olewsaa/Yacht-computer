#!/usr/bin/env python
import time, socket
# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# socket.AF_INET is  Internet
# socket.SOCK_DGRAM) is  UDP
SignalK_UPD_PORT=20220
SignalK_IP='192.168.1.169'
print("Simulate SignalK")

while True:
#    t=input("Please enter a number: ")
#    x = float(t)
    d=135+int(time.strftime("%S"))
    s=5.3+0.2*int(time.strftime("%S"))
    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"environment.wind.angleApparent","value":'+format(d,'5.3f')+'}]}]}'
    print(SignalK)
    sock.sendto(SignalK, (SignalK_IP, SignalK_UPD_PORT))
    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"environment.wind.speedApparent","value":'+format(s,'5.3f')+'}]}]}'
    print(SignalK)
    sock.sendto(SignalK, ('192.168.1.169', SignalK_UPD_PORT))
    time.sleep(2)
    

# SignalK :
# environment.wind.speedApparent
# environment.wind.angleApparent
