#!/usr/bin/env python3
import os, json, requests

# Script to set the time and date from SignalK. The RPi have no internal
# real time battery powered clock to keep time. Without internet
# connection it cannot set its clock. When a separate GPS is not
# connected it ned to ask the SignalK server for time data. 

#
# Setting UTC timezone via CLI :
# sudo rm /etc/localtime
# sudo ln -s  /usr/share/zoneinfo/UTC  /etc/localtime
# 
# timedatectl list-timezones
#
# UTC or Europe/Oslo
# 
# sudo timedatectl set-timezone Europe/Oslo
# sudo timedatectl set-timezone UTC
#

resp = requests.get('http://demo.signalk.org/signalk/v1/api/vessels/self/navigation/datetime/value', verify=False)

data = json.loads(resp.content)
#print(data)
cmd="sudo date -s "+data
os.system(cmd)


