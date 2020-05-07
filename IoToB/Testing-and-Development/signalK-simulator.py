#!/usr/bin/env python
import time, socket
# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# socket.AF_INET is  Internet
# socket.SOCK_DGRAM) is  UDP
SignalK_UPD_PORT=55557
SignalK_IP='192.168.1.169'
print("Simulate SignalK")

while True:
#    t=input("Please enter a number: ")
#    x = float(t)
    d=6.0*int(time.strftime("%S"))
# SignalK uses radians, convert to degrees.    
    d=d/180.0*3.141592 
    s=5.3+0.4*int(time.strftime("%S"))
    print("send data ",format((d/3.14*180.0),'5.2f'),format(s,'5.2f'))
    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"environment.wind.angleApparent","value":'+format(d,'5.3f')+'}]}]}'
    #print(SignalK)
    sock.sendto(SignalK, (SignalK_IP, SignalK_UPD_PORT))
    SignalK='{"updates": [{"$source": "OrangePi","values":[ {"path":"environment.wind.speedApparent","value":'+format(s,'5.3f')+'}]}]}'
    #print(SignalK)
    sock.sendto(SignalK, ('192.168.1.169', SignalK_UPD_PORT))
    time.sleep(2)
    

# SignalK :
# environment.wind.speedApparent
# environment.wind.angleApparent
#
# Remember to set up a SignalK input at port 55557 using the SignalK web interface.
# Meny, Server, Connections, Add. Input type SignalK, Enabled yes, ID "Simulator",
# Source UDP, Port 55557, No Self mapping.
#
