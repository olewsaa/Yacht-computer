# Settings for Raspberry Pi and OpenCPN

## Config settings

The screen settings can be a trial and error exercise. Which OpenGL setting and other display related 
settings to set are not intuitive and some hints can reduce the trial and error step. As there are both 
HDMI to HDMI, HDI to DVI and other cables and monitors availale there is not a "one size fit all" setting.

Open the Raspberry menu, preferences and old raspi-config.
Select option 7 advances options and open it.
- Overscan, this can help if the screen appear to large for the display. I had to set this option when 
connecting to a TV HDMI input.
- Memory split, the recomendation is to use 64 MB. There are only 1024 MiB in total so setting this too
high may reduce the amount of memory for OpenCPN and OS.
- Resolution, a monitor might communcate with the GPU (graphical processing Unit) and suggest a default. 
In other cases you migth select a suitable resolution, a HD TV can tackle 1920x1280. 
- GL driver, OpenGL is needed, both Full KMS driver and Fake KMS driver works, suggestions from gamers 
is to use the full KMS.
