import tkinter
from time import sleep

main = tkinter.Tk()
txt = tkinter.Text(main)
txt.grid()
def update_txt(event = None):
    vals = ['This is some text.',
        'This is some more.',
        'Blah blah blah']
    i = 0
    while i < len(vals):
        txt.delete('1.0','end')
        txt.insert('1.0',vals[i])
        txt.update_idletasks()
        sleep(2)
        i=i+1

main.after(1000,update_txt)
main.mainloop()
