from tkinter import *
import os
import glob
import RPi.GPIO as GPIO

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')


GPIO.setmode(GPIO.BOARD)
RED_LED = 18
GRE_LED = 16
BLU_LED = 12
GPIO.setup(RED_LED, GPIO.OUT)
GPIO.setup(GRE_LED, GPIO.OUT)
GPIO.setup(BLU_LED, GPIO.OUT)

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
    t1=read_temp(1)
    t2=read_temp(2)
    if t1<20:
            GPIO.output(BLU_LED, True)
            GPIO.output(GRE_LED, False)
            GPIO.output(RED_LED, False)
    if t1>20 and t1<23:
            GPIO.output(BLU_LED, False)
            GPIO.output(GRE_LED, True)
            GPIO.output(RED_LED, False)
            
    if t1>23:
            GPIO.output(BLU_LED, False)
            GPIO.output(GRE_LED, False)
            GPIO.output(RED_LED, True)    
    msg =   "Temp1: "+"{:3.1f}".format(t1)+"\u2103\n"
    msg=msg+"Temp2: "+"{:3.1f}".format(t1)+"\u2103"
#    print(msg)
    v.set(msg)    
    root.after(2000, updateTime)

v = StringVar()
Label(root, textvariable=v, font=("Helvetica", 30)).pack()
v.set('--.-')

root.after(2, updateTime)
root.mainloop()

