#!/usr/bin/env python

import time, socket

# Import SPI library (for hardware SPI) and MCP3008 library.
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008


# Initiate socket 
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Software SPI configuration:
# CLK  = 18
# MISO = 23
# MOSI = 24
# CS   = 25
# mcp = Adafruit_MCP3008.MCP3008(clk=CLK, cs=CS, miso=MISO, mosi=MOSI)

# Hardware SPI configuration:
SPI_PORT   = 0
SPI_DEVICE = 0
mcp = Adafruit_MCP3008.MCP3008(spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE))

SignalK=""

while True:
    # Grab the difference between channel 0 and 1 (i.e. channel 0 minus 1).
    # Note you can specify any value in 0-7 to grab other differences:
    #  - 0: Return channel 0 minus channel 1
    #  - 1: Return channel 1 minus channel 0
    #  - 2: Return channel 2 minus channel 3
    #  - 3: Return channel 3 minus channel 2
    #  - 4: Return channel 4 minus channel 5
    #  - 5: Return channel 5 minus channel 4
    #  - 6: Return channel 6 minus channel 7
    #  - 7: Return channel 7 minus channel 6
    for k in range(2):
        # Should have made an array of names and values, but there is only two,
        # quick cut/paste is faster.
        if  k == 0:  
            value = mcp.read_adc_difference(1)
            i=float((value)/1023.0)*21.0
            #print 'Channel 0 minus 1: {0}'.format(value),' equal ',format(i,'.2f'),'A'
            SignalK='{"updates": [{"$source": "SPI.MCP3008","values":[ {"path": "electrical.WCS1800.current","value":'+format(i,'5.3f')+'}]}]}'
            #print SignalK
            sock.sendto(SignalK, ('127.0.0.1', 55557))
        if k == 1:
            value = mcp.read_adc_difference(3)
            i=float((value)/1023.0)*12.0
            #print 'Channel 3 minus 2: {0}'.format(value),' equal ',format(i,'.2f'),'A'
            SignalK='{"updates": [{"$source": "SPI.MCP3008","values":[ {"path": "electrical.ACS712.current","value":'+format(i,'5.3f')+'}]}]}'
            #print SignalK
            sock.sendto(SignalK, ('127.0.0.1', 55557))    
    time.sleep(1)

    

     
