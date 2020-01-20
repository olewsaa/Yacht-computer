#
# A simple script to explore text command lines to scan and connect to access points.
# 
# 

import os
import subprocess
import time 

c={}
print("Scanning wifi access points....")
p=subprocess.check_output('wpa_cli -i wlan1  scan', shell=True)
for i in range(2):
        time.sleep(1)
        print(".",end="", flush=True)
print()
a=subprocess.check_output('wpa_cli -i wlan1  scan_results', shell=True) 
b=a.splitlines(True)

for i in range(len(b)):
	c[i]=b[i].split()

for i in  range(1,len(b)):
	if len(c[i])>4:
                ap_name=str(c[i][4]).replace("b'","'").replace("'","")
                ap_signal=-1200/int(str(c[i][2]).replace("b'","'").replace("'",""))
#		print(i-1,str(c[i][4]).replace("b'","'").replace("'",""),'\t',\
#		(-1/int(str(c[i][2]).replace("b'","'").replace("'","")))*100)
                print(i-1,ap_name, "%.2f" % ap_signal)


print("")
p=subprocess.check_output('wpa_cli -i wlan1  list_networks', shell=True)
b=p.splitlines(True)
for i in range(len(b)):
	c[i]=b[i].split()
	#print(c[i])
	if (len(c[i])>3):
		if (str(c[i][3]).find("CURRENT")==3):
			print("Active  network is : ",\
			str(c[i][1]).replace("b'","'").replace("'",""))



name=input("Connect to network  >")
p=subprocess.check_output('wpa_cli -i wlan1  list_networks', shell=True)
print(p)
if (str(p).find(name)>0):
	print("known")
	b=p.splitlines(True)
	for j in range(1,len(b)):
		#print(b[j],type(b[j]))
		#print(str(b[j]).find(name))
		if str(b[j]).find(name)>0:
			line=b[j].split()
			#line=str(p.splitlines()[0]).replace("b'","").replace("'","")
			#print(str(line[0]).replace("b'","").replace("'",""))
			no=(str(line[0]).replace("b'","").replace("'",""))
			print("Number ", no)
else:
	pw=input("Password >")
	print("Trying to connect to :",name)
	p=subprocess.check_output('wpa_cli -i wlan1  add_network', shell=True)
	no=int(str(p.splitlines()[0]).replace("b'","").replace("'",""))
	cmd="wpa_cli -i wlan1 set_network "+str(no)+" ssid "+"'\""+name+"\"'"
	p=subprocess.check_output(cmd, shell=True)
	if str(p).find("OK")<0:
		print("Failed on setting network name, exit")
		exit()
	for j in range(4):
		cmd="wpa_cli -i wlan1 set_network "+str(no)+" psk "+"'\""+pw+"\"'"
		p=subprocess.check_output(cmd, shell=True)
		if str(p).find("OK")<0:
        		print("Failed on setting network password")
        		pw=input("Password >")
		else:
			print("Password ok")
			p=subprocess.check_output('wpa_cli -i wlan1 save_config', shell=True)
			break



cmd="wpa_cli -i wlan1  disconnect"
print(cmd)
time.sleep(5)
p=subprocess.check_output(cmd, shell=True)
cmd="wpa_cli -i wlan1 enable_network "+str(no)
print(cmd)
time.sleep(3)
p=subprocess.check_output(cmd, shell=True)
cmd="wpa_cli -i wlan1  reconnect"
print(cmd)
p=subprocess.check_output(cmd, shell=True)


time.sleep(5)
p=subprocess.check_output('ifconfig wlan1', shell=True)
while str(p).find("broadcast")<0:
	time.sleep(1)
	print(".",end="", flush=True)
	p=subprocess.check_output('ifconfig wlan1', shell=True)
print()
b=p.splitlines(True)
c=str(b[1]).split()
ip=c[2].replace("b'","'").replace("'","")
print("Assigned ip number on network ",name," is ",ip)











