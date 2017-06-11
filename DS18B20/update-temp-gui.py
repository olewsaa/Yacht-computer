import tkinter
from tkinter import ttk
import os
import glob
import time

main = tkinter.Tk()
Temps = tkinter.Text(main)
main.title('Temperaturer')
main.option_add('*tearOff', 'FALSE')        
Temps.grid(column=0, row=0, sticky='nsew')

#Temps.grid()

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')
 
device_folder=[1,2,3]
device_file=[1,2,3]

base_dir = '/sys/bus/w1/devices/'

for j in 1,2:
	print(j)
	device_folder[j] = glob.glob(base_dir + '28*')[j-1]
	print("device_folder", device_folder[j])
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


def update_temps(event = None):
        Temps.temp1_label['text'] = read_temp(1)
        Temps.temp2_label['text'] = read_temp(2)
        Temps.update_idletasks()
        print("hei")
        Temps.after(2000,update_temps)


ttk.Separator(Temps, orient='horizontal').grid(column=0,
                row=1, columnspan=4, sticky='ew')
for child in Temps.winfo_children():
        child.grid_configure(padx=30, pady=0)
#        print(child)
        
        
Temps.temp1_frame = ttk.LabelFrame(main, text='Temp1', height=100)
Temps.temp1_frame.grid(column=0, row=4, columnspan=4, sticky='nesw') 
Temps.temp1_label = ttk.Label(Temps.temp1_frame, text='')
Temps.temp1_label.grid(column=0, row=0)

Temps.temp2_frame = ttk.LabelFrame(main, text='Temp2', height=100)
Temps.temp2_frame.grid(column=1, row=4, columnspan=4, sticky='nesw')
Temps.temp2_label = ttk.Label(Temps.temp2_frame, text='')
Temps.temp2_label.grid(column=1, row=0)

Temps.temp1_label['text'] = read_temp(1)
Temps.temp2_label['text'] = read_temp(2)

update_temps()
main.mainloop()
