#!/bin/bash

logdir=Yacht-computer/LOGS/

dtg=$(date +%F_%H-%M)
logfile=algoldata-${dtg}.log
python /home/ole/python/readtemp.py > $logfile
/home/ole/python/DHT/hum.sh >> $logfile
python  /home/ole/python/level.py >> $logfile
rsync -s $logfile  ole@gina.domain.no:$logdir
rm $logfile 

/home/ole/python/lyspaa.py
sleep 1
imgfile=algolimage-${dtg}.jpeg
streamer -s 640x480 -o $imgfile
rsync -s $imgfile ole@gina.domain.no:$logdir
rm $imgfile
/home/ole/python/lysav.py

imgfile=algol-ute-${dtg}.jpg
fswebcam -r 1280x720 -d /dev/video2 $imgfile
rsync -s $imgfile  ole@gina.domain.no:$logdir
rm $imgfile



