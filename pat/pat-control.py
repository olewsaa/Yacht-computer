#!/usr/bin/env python3
import os
import time
import tkinter as tk
from subprocess import check_output, CalledProcessError


def check_pid(name):    
    try:
        p = check_output(["pidof",name]).split()
    except  CalledProcessError:
        p = str(0)        
    return p


def start_pat():
    for r in 1,2,3 :
        tk.Label(master, text="                     ",anchor="e").grid(row=r)
    tk.Label(master, text="Starting Pat",anchor="e").grid(row=0)
    os.system("/home/pi/pat/pat.start")
    time.sleep(3)
    display_pids()	
    
    
def stop_pat():
    for r in 1,2,3 :
        tk.Label(master, text="                      ",anchor="e").grid(row=r)
    tk.Label(master, text="Stopping Pat",anchor="e").grid(row=0)    
    os.system("/home/pi/pat/pat.stop")
    display_pids()	


def display_pids():
    tk.Label(master, text="Services PIDs",anchor="e").grid(row=0)
    tk.Label(master, text="pid rigctl      ").grid(row=1)
    tk.Label(master, text="pid piardopc    ").grid(row=2)
    tk.Label(master, text="pid pat         ").grid(row=3)
    e1 = tk.Entry(master, width="7")
    e2 = tk.Entry(master, width="7")
    e3 = tk.Entry(master, width="7")
    e1.insert(0, check_pid("rigctld"))
    e2.insert(0, check_pid("piardopc"))
    e3.insert(0, check_pid("pat"))
    e1.grid(row=1, column=1)
    e2.grid(row=2, column=1)
    e3.grid(row=3, column=1)

    
    
master = tk.Tk()
master.title("Simple Pat control")

tk.Button(master, text='Start', height="2",
          command=start_pat).grid(row=4, column=0, sticky=tk.W, pady=1)

tk.Button(master, text='Stop', height="2",
          command=stop_pat).grid(row=4, column=1, sticky=tk.W, pady=1)

tk.Button(master, text='Show pids', height="2",
          command=display_pids).grid(row=4, column=2, sticky=tk.W, pady=1)

tk.Button(master, text='Quit', height="2",
          command=master.quit).grid(row=4, column=3, sticky=tk.W, pady=1)


master.mainloop()


