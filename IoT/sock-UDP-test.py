import time, socket

# A simple test to check if the ESP board communicates correct, 
# also to debug SignalK messages.

# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# socket.AF_INET is  Internet
# socket.SOCK_DGRAM is  UDP
# socket.SOCK_STREAM is  TCP

print("Trying to receive values as SignalK")

server_address = ('192.168.1.111', 10002)
sock.bind(server_address)

#sock.listen(1)
while True:
    data, addr = sock.recvfrom(128)
    print "from", addr
    print data

