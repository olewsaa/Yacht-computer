# Testing

The scripts and files here are small snippets that can be handy test test and to learn about what's going on.

All the sofware could have been written in Python, but Arduino IDE with C is familiar and hence the usage of
the C language. Using micropython directly on some microcontroller is possibly the future, and is a nice
way to develop. I have not tested [micropython on ESP328](https://randomnerdtutorials.com/getting-started-micropython-esp32-esp8266/) (only STM 32), but it should work and make development work even simpler.

Early on I wrote the SignalK line in Python, and translated to an Arduino sketch. The proper way would have 
been use a class with JSON object functions. The manual SignalK line stuck and for my small and simple messages 
sent to SignalK server.
