# Changes to the SoftwareSerial library

## Background 

The simple examples would not build using the following versions of
the build invironment.

- Arduino IDE 1.8.12 
- ESP8266 library 2.7.1


## Fix a building issue 

The SoftwareSerial library with 9 bit support failed to build applications
with the current setup.

One line needed to be changed  in the file EspSoftwareSerial/src/SoftwareSerial.cpp :
* //attachInterruptArg(digitalPinToInterrupt(m_rxPin), (void (*)())(rxBitISR), this, CHANGE);
* Attachinterruptarg(digitalPinToInterrupt(m_rxPin), reinterpret_cast<void (*)(void*)>(rxBitISR), this, CHANGE);

An error in the casting of void function and void parameters. Using the 
same line from the initial 8 bit version worked. 
