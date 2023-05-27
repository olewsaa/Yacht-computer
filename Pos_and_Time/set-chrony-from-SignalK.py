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

#resp = requests.get('http://10.10.10.1:3000/signalk/v1/api/vessels/self/navigation/datetime/value', verify=False)


data = json.loads(resp.content)
print(data)
#cmd="sudo date -s "+data
#os.system(cmd)
Y=data[0:4]
M=data[5:7]
D=data[8:10]
h=data[11:13]
m=data[14:16]
s=data[17:24]
#print(D,"/",M,"-",Y," ",h,":",m,":",s)
#
sec='{:.0f}'.format(float(s))
str=Y+"-"+M+"-"+D+"T"+h+":"+m+":"+'{:0>2}'.format(sec)
cmd="sudo chronyc settime "+str
print(cmd)
#os.system(cmd)
