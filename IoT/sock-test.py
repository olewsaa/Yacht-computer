# A simple socket test that can come in handly when playing with ESP2866 and ESP32s.

import socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# AF_INET Internet IP
# SOCK_STREAM TCP


server_address = ('192.168.1.111', 10002)
sock.bind(server_address)
sock.listen(1)
while True:
    connection, client_address = sock.accept()
#    print connection
    data = connection.recv(128)
    print data


connection.close()
