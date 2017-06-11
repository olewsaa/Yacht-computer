"""
temp-gui.py
~~~~~~
 
Creates a simple GUI for displaying two temps.
"""
 
import tkinter
from tkinter import ttk

import os
import glob
import time

class Temps(ttk.Frame):
    """The temp gui and functions."""
    def __init__(self, parent, *args, **kwargs):
        ttk.Frame.__init__(self, parent, *args, **kwargs)
        self.root = parent
        self.init_gui()

        
    def get_data(self):
        self.temp1_label['text'] = read_temp(1)
        self.temp2_label['text'] = read_temp(2)


    def on_quit(self):
        """Exits program."""
        quit()
 
    def init_gui(self):
        """Builds GUI."""
        self.root.title('Temperaturer')
        self.root.option_add('*tearOff', 'FALSE')        
        self.grid(column=0, row=0, sticky='nsew')
        self.menubar = tkinter.Menu(self.root)
        self.menu_file = tkinter.Menu(self.menubar)
        self.menu_file.add_command(label='Exit', command=self.on_quit) 
        self.menubar.add_cascade(menu=self.menu_file, label='File')
        self.root.config(menu=self.menubar)
        
        self.temp1_frame = ttk.LabelFrame(self, text='Temp', height=100)
        self.temp1_frame.grid(column=0, row=4, columnspan=4, sticky='nesw')
 
        self.temp1_label = ttk.Label(self.temp1_frame, text='')
        self.temp1_label.grid(column=0, row=0)

        self.temp2_frame = ttk.LabelFrame(self, text='Temp', height=100)
        self.temp2_frame.grid(column=1, row=4, columnspan=4, sticky='nesw')
 
        self.temp2_label = ttk.Label(self.temp2_frame, text='')
        self.temp2_label.grid(column=1, row=0)
        
        self.tempdata()
                

        # Labels that remain constant throughout execution.
        ttk.Label(self, text='Temperaturer').grid(column=0, row=0,
               columnspan=4)
        ttk.Label(self, text='  Pult  ').grid(column=0, row=2, sticky='w')
        ttk.Label(self, text='  Motor ').grid(column=2, row=2,  sticky='w')
        ttk.Separator(self, orient='horizontal').grid(column=0,
                row=1, columnspan=4, sticky='ew')
        for child in self.winfo_children():
            child.grid_configure(padx=5, pady=5)
        
    def tempdata(self):
        self.temp1_frame = ttk.LabelFrame(self, text='Temp1', height=100)
        self.temp1_frame.grid(column=0, row=4, columnspan=4, sticky='nesw') 
        self.temp1_label = ttk.Label(self.temp1_frame, text='')
        self.temp1_label.grid(column=0, row=0)
        self.temp2_frame = ttk.LabelFrame(self, text='Temp2', height=100)
        self.temp2_frame.grid(column=1, row=4, columnspan=4, sticky='nesw')
        self.temp2_label = ttk.Label(self.temp2_frame, text='')
        self.temp2_label.grid(column=1, row=0)
        self.temp1_label['text'] = read_temp(1)
        self.temp2_label['text'] = read_temp(2)

    def update(event = None):
        self.update_idletasks()
        self.temp1_label['text'] = read_temp(1)
        self.temp2_label['text'] = read_temp(2)
        self.after(2000,self.update)   
    
         

# program start here

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
	

temp=[1,2,3]


print(read_temp(1))
print(read_temp(2))

if __name__ == '__main__':
    root = tkinter.Tk()
    Temps(root)
    root.update()
    root.mainloop()

