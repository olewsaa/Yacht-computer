from tkinter import *
import os
import glob

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

device_folder=[1,2,3]
device_file=[1,2,3]

base_dir = '/sys/bus/w1/devices/'

for j in 1,2:
#	print(j)
	device_folder[j] = glob.glob(base_dir + '28*')[j-1]
#	print("device_folder", device_folder[j])
	device_file[j] = device_folder[j] + '/w1_slave'
 
def read_temp_raw(j):
    f = open(device_file[j], 'r')
    lines = f.readlines()
    f.close()
    return lines
 
def read_temp(j):
    lines = read_temp_raw(j)
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw(j)
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        return temp_c


root = Tk()
root.title('Temperaturer')

def updateTime() :
    msg =   "Temp1: "+"{:3.1f}".format(read_temp(1))+"\u2103\n"
    msg=msg+"Temp2: "+"{:3.1f}".format(read_temp(2))+"\u2103"
#    print(msg)
    v.set(msg)
    root.after(2000, updateTime)

v = StringVar()
Label(root, textvariable=v, font=("Helvetica", 30)).pack()
v.set('--.-')

root.after(2, updateTime)
root.mainloop()

