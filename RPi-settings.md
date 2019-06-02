# Settings for Raspberry Pi and OpenCPN

## Config settings

### Screen / Display
The screen settings can be a trial and error exercise. Which OpenGL setting and other display related 
settings to set are not intuitive and some hints can reduce the trial and error step. As there are both 
HDMI to HDMI, HDI to DVI and other cables and monitors availale there is not a "one size fit all" setting.
Some help can be found here: [using TV as monitor](https://www.pcworld.com/article/2924203/use-your-tv-as-a-computer-monitor-everything-you-need-to-know.html).

Open the Raspberry menu, preferences and old raspi-config.
Select option 7 advances options and open it.
- Overscan, this can help if the screen appear too large for the display. I had to set this option when 
connecting to a TV HDMI input (using a computer terminal with HDMI=>DVI adaper is fixes this automatic).
- Memory split, the recommendation is to use 64 MiB. There are only 1024 MiB in total so setting this too
high may reduce the amount of memory for OpenCPN and OS. Some gamers suggest 256 MiB.
- Resolution, a monitor might communicate with the GPU (graphical processing Unit) and suggest a default. 
In other cases you might select a suitable resolution, a HD TV can tackle 1920x1080. 
- GL driver, OpenGL is needed, both Full KMS driver and Fake KMS driver works, suggestions from gamers 
is to use the full KMS.

To test the OpenGL driver the mesa-utils come in handy. Install using `apt install mesa-utils`. Starting the nice 
simple test glxgears should produce cogwheels running smoothly on the display. In my system it performs at about 60 
frames per second.


### Pi Configuration
The graphical config tool offers a range of settings. It is generally a good idea to disable interfaces services that
are not needed. I have turned off the following services:
- camera
- VNC
- SPI 
- Serial
- 1-wire
The important one is secure shell, SSH. Set this to enable if you want to login to the system. 

### Lost access point after upgrade
This topic is covered i the section about RPi config files. After an upgrade I lost the 
access point set by OpenPlotter. It took some manoeuvring to get things back. 
