#!/usr/bin/python

import os
from Tkinter import *

def quit():
    global ae_iso_var
    del(ae_iso_var)
    top.destroy()

def quit_bind(e):
    quit()

top = Tk()
top['pady'] = 5
top['padx'] = 5
top.title('Guzzi Camera Control')
top.bind('<q>', quit_bind)
top.protocol("WM_DELETE_WINDOW", quit)

###########################################################
### AE Frame ##############################################
###########################################################
AE_MODE_AUTO = 0
AE_MODE_MANUAL = 1

### Vars ###
ae_mode_var                 = IntVar()
ae_send_on_change_var       = IntVar()
ae_exp_var                  = IntVar()
ae_exp_send_on_change_var   = IntVar()
ae_iso_var                  = IntVar()
ae_iso_send_on_change_var   = IntVar()

### Functions ###
def ae_set_auto():
    shell_command = 'adb shell \"echo 8 > /dev/myriad1\"'
    print shell_command
    os.system(shell_command)

def ae_set_manual(exp, iso, frame_duration):
    shell_command = 'adb shell \"echo 7 %d %d %d > /dev/myriad1\"' % (
            exp,
            iso,
            frame_duration
        )
    print shell_command
    os.system(shell_command)

def ae_set():
    if AE_MODE_MANUAL == ae_mode_var.get():
        ae_set_manual(ae_exp_var.get(), ae_iso_var.get(), 67000)
    else:
        ae_set_auto()

def ae_exp_command(pos):
    if ae_exp_send_on_change_var.get():
        ae_set()

def ae_iso_command(pos):
    if ae_iso_send_on_change_var.get():
        ae_set()

def ae_exp_val_add(val):
    ae_exp_scale.set(ae_exp_scale.get() + val)

def ae_iso_val_add(val):
    ae_iso_scale.set(ae_iso_scale.get() + val)

def ae_send_command():
    ae_set()

def ae_mode_var_change(name, index, mode):
    for w in ae_manual_widgets:
        w['state'] = NORMAL if AE_MODE_MANUAL == ae_mode_var.get() else DISABLED
    if ae_send_on_change_var.get():
        ae_set()

def ae_exp_entry_var_change(name, index, mode):
    ae_exp_scale['label'] = 'Exposure: %dus' % ae_exp_var.get()

def ae_iso_entry_var_change(name, index, mode):
    ae_iso_scale['label'] = 'ISO: %d' % ae_iso_var.get()

### Vras trace ###
ae_mode_var.trace('w', ae_mode_var_change)
ae_exp_var.trace("w", ae_exp_entry_var_change)
ae_iso_var.trace("w", ae_iso_entry_var_change)

### AE Frame ###
ae_frame = LabelFrame(top)
ae_frame['text'] = 'AE'
ae_frame['padx'] = 5
ae_frame['pady'] = 5

### Mode ###
ae_exp_radio_auto = Radiobutton(ae_frame)
ae_exp_radio_auto['text'] = 'Auto'
ae_exp_radio_auto['variable'] = ae_mode_var
ae_exp_radio_auto['value'] = 0

ae_exp_radio_manual = Radiobutton(ae_frame)
ae_exp_radio_manual['text'] = 'Manual'
ae_exp_radio_manual['variable'] = ae_mode_var
ae_exp_radio_manual['value'] = 1

### Send on change ###
ae_send_on_change = Checkbutton(ae_frame)
ae_send_on_change['text'] = 'Send on change'
ae_send_on_change['variable'] = ae_send_on_change_var

### Exposure scale ###
ae_exp_scale = Scale(ae_frame)
ae_exp_scale['orient'] = HORIZONTAL
ae_exp_scale['from_'] = 0
ae_exp_scale['to'] = 66666
ae_exp_scale['variable'] = ae_exp_var
ae_exp_scale['resolution'] = 1
ae_exp_scale['showvalue'] = 0
ae_exp_scale['command'] = ae_exp_command

ae_exp_scale.bind('<Right>', lambda e: ae_exp_val_add(  0))
ae_exp_scale.bind('<Up>'   , lambda e: ae_exp_val_add(  2))
ae_exp_scale.bind('<k>'    , lambda e: ae_exp_val_add(  1))
ae_exp_scale.bind('<l>'    , lambda e: ae_exp_val_add(  1))
ae_exp_scale.bind('<Prior>', lambda e: ae_exp_val_add( 10))
ae_exp_scale.bind('<Left>' , lambda e: ae_exp_val_add(- 0))
ae_exp_scale.bind('<Down>' , lambda e: ae_exp_val_add(- 2))
ae_exp_scale.bind('<h>'    , lambda e: ae_exp_val_add(- 1))
ae_exp_scale.bind('<j>'    , lambda e: ae_exp_val_add(- 1))
ae_exp_scale.bind('<Next>' , lambda e: ae_exp_val_add(-10))
ae_exp_scale.bind('<MouseWheel>', lambda e: ae_exp_val_add(e.delta/120))
ae_exp_scale.bind('<Button-1>'  , lambda e: ae_exp_scale.focus_set())
ae_exp_scale.bind('<Button-4>'  , lambda e: ae_exp_val_add(  1))
ae_exp_scale.bind('<Button-5>'  , lambda e: ae_exp_val_add(- 1))

ae_exp_send_on_change = Checkbutton(ae_frame)
ae_exp_send_on_change['text'] = 'Send on change'
ae_exp_send_on_change['variable'] = ae_exp_send_on_change_var

### ISO scale ###
ae_iso_scale = Scale(ae_frame)
ae_iso_scale['orient'] = HORIZONTAL
ae_iso_scale['from_'] = 100
ae_iso_scale['to'] = 6400
ae_iso_scale['variable'] = ae_iso_var
ae_iso_scale['resolution'] = 1
ae_iso_scale['showvalue'] = 0
ae_iso_scale['command'] = ae_iso_command

ae_iso_scale.bind('<Right>', lambda e: ae_iso_val_add(  0))
ae_iso_scale.bind('<Up>'   , lambda e: ae_iso_val_add(  2))
ae_iso_scale.bind('<k>'    , lambda e: ae_iso_val_add(  1))
ae_iso_scale.bind('<l>'    , lambda e: ae_iso_val_add(  1))
ae_iso_scale.bind('<Prior>', lambda e: ae_iso_val_add( 10))
ae_iso_scale.bind('<Left>' , lambda e: ae_iso_val_add(- 0))
ae_iso_scale.bind('<Down>' , lambda e: ae_iso_val_add(- 2))
ae_iso_scale.bind('<h>'    , lambda e: ae_iso_val_add(- 1))
ae_iso_scale.bind('<j>'    , lambda e: ae_iso_val_add(- 1))
ae_iso_scale.bind('<Next>' , lambda e: ae_iso_val_add(-10))
ae_iso_scale.bind('<MouseWheel>', lambda e: ae_iso_val_add(e.delta/120))
ae_iso_scale.bind('<Button-1>'  , lambda e: ae_iso_scale.focus_set())
ae_iso_scale.bind('<Button-4>'  , lambda e: ae_iso_val_add(  1))
ae_iso_scale.bind('<Button-5>'  , lambda e: ae_iso_val_add(- 1))

ae_iso_send_on_change = Checkbutton(ae_frame)
ae_iso_send_on_change['text'] = 'Send on change'
ae_iso_send_on_change['variable'] = ae_iso_send_on_change_var

### Send button ###
ae_send_button = Button(ae_frame)
ae_send_button['command'] = ae_send_command
ae_send_button['text'] = 'Send'

### Common ###
ae_manual_widgets = [
        ae_exp_scale,
        ae_exp_send_on_change,
        ae_iso_scale,
        ae_iso_send_on_change
    ]

### Default values ###
ae_mode_var.set(AE_MODE_AUTO)
ae_send_on_change_var.set(0)
ae_exp_var.set(33333)
ae_iso_var.set(100)
ae_exp_send_on_change_var.set(0)
ae_iso_send_on_change_var.set(0)

### Grid ###
ae_exp_radio_auto.grid      (in_=ae_frame, row=0, column=0,                 sticky=W        )
ae_exp_radio_manual.grid    (in_=ae_frame, row=1, column=0,                 sticky=W        )
ae_send_on_change.grid      (in_=ae_frame, row=0, column=1,                 sticky=W        )
ae_exp_scale.grid           (in_=ae_frame, row=2, column=0,                 sticky=W+E+N+S  )
ae_exp_send_on_change.grid  (in_=ae_frame, row=2, column=1,                 sticky=W+S      )
ae_iso_scale.grid           (in_=ae_frame, row=3, column=0,                 sticky=W+E+N+S  )
ae_iso_send_on_change.grid  (in_=ae_frame, row=3, column=1,                 sticky=W+S      )
ae_send_button.grid         (in_=ae_frame, row=4, column=0, columnspan=2,   sticky=W+E+N+S  )

ae_frame.columnconfigure(0, weight=1)

ae_frame.rowconfigure(0, weight=0)
ae_frame.rowconfigure(1, weight=0)
ae_frame.rowconfigure(2, weight=0)
ae_frame.rowconfigure(3, weight=0)
ae_frame.rowconfigure(4, weight=0)

ae_exp_scale['length'] = 300 # TODO: Calc this size

###########################################################
### AF Frame ##############################################
###########################################################
AF_MODE_AUTO = 0
AF_MODE_MANUAL = 1

### Vars ###
af_mode_var                 = IntVar()
af_send_on_change_var       = IntVar()
af_pos_var                  = IntVar()
af_pos_send_on_change_var   = IntVar()

### Functions ###
def af_set_auto():
    shell_command = 'adb shell \"echo 4 > /dev/myriad1\"'
    print shell_command
    os.system(shell_command)

def af_set_manual(pos):
    shell_command = 'adb shell \"echo 3 %d > /dev/myriad1\"' % (
            pos
        )
    print shell_command
    os.system(shell_command)

def af_set():
    if AF_MODE_MANUAL == af_mode_var.get():
        af_set_manual(af_pos_var.get())
    else:
        af_set_auto()

def af_pos_command(pos):
    if af_pos_send_on_change_var.get():
        af_set()

def af_pos_val_add(val):
    af_pos_scale.set(af_pos_scale.get() + val)

def af_send_command():
    af_set()

def af_mode_var_change(name, index, mode):
    for w in af_manual_widgets:
        w['state'] = NORMAL if AF_MODE_MANUAL == af_mode_var.get() else DISABLED
    if af_send_on_change_var.get():
        af_set()

def af_pos_entry_var_change(name, index, mode):
    af_pos_scale['label'] = 'Focus pos: %d' % af_pos_var.get()

### Vras trace ###
af_mode_var.trace('w', af_mode_var_change)
af_pos_var.trace("w", af_pos_entry_var_change)

### AF Frame ###
af_frame = LabelFrame(top)
af_frame['text'] = 'AF'
af_frame['padx'] = 5
af_frame['pady'] = 5

### Mode ###
af_radio_auto = Radiobutton(af_frame)
af_radio_auto['text'] = 'Auto and trigger'
af_radio_auto['variable'] = af_mode_var
af_radio_auto['value'] = AF_MODE_AUTO

af_radio_manual = Radiobutton(af_frame)
af_radio_manual['text'] = 'Manual'
af_radio_manual['variable'] = af_mode_var
af_radio_manual['value'] = AF_MODE_MANUAL

af_send_on_change = Checkbutton(af_frame)
af_send_on_change['text'] = 'Send on change'
af_send_on_change['variable'] = af_send_on_change_var

### Exposure scale ###
af_pos_scale = Scale(af_frame)
af_pos_scale['orient'] = HORIZONTAL
af_pos_scale['from_'] = 0
af_pos_scale['to'] = 255
af_pos_scale['variable'] = af_pos_var
af_pos_scale['resolution'] = 1
af_pos_scale['showvalue'] = 0
af_pos_scale['command'] = af_pos_command

af_pos_scale.bind('<Right>', lambda e: af_pos_val_add(  0))
af_pos_scale.bind('<Up>'   , lambda e: af_pos_val_add(  2))
af_pos_scale.bind('<k>'    , lambda e: af_pos_val_add(  1))
af_pos_scale.bind('<l>'    , lambda e: af_pos_val_add(  1))
af_pos_scale.bind('<Prior>', lambda e: af_pos_val_add( 10))
af_pos_scale.bind('<Left>' , lambda e: af_pos_val_add(- 0))
af_pos_scale.bind('<Down>' , lambda e: af_pos_val_add(- 2))
af_pos_scale.bind('<h>'    , lambda e: af_pos_val_add(- 1))
af_pos_scale.bind('<j>'    , lambda e: af_pos_val_add(- 1))
af_pos_scale.bind('<Next>' , lambda e: af_pos_val_add(-10))
af_pos_scale.bind('<MouseWheel>', lambda e: af_pos_val_add(e.delta/120))
af_pos_scale.bind('<Button-1>'  , lambda e: af_pos_scale.focus_set())
af_pos_scale.bind('<Button-4>'  , lambda e: af_pos_val_add(  1))
af_pos_scale.bind('<Button-5>'  , lambda e: af_pos_val_add(- 1))

af_pos_send_on_change = Checkbutton(af_frame)
af_pos_send_on_change['text'] = 'Send on change'
af_pos_send_on_change['variable'] = af_pos_send_on_change_var

### Send button ###
af_send_button = Button(af_frame)
af_send_button['command'] = af_send_command
af_send_button['text'] = 'Send'

### Common ###
af_manual_widgets = [
        af_pos_scale,
        af_pos_send_on_change,
    ]

### Default values ###
af_mode_var.set(AF_MODE_AUTO)
af_pos_var.set(0)
af_pos_send_on_change_var.set(0)

### Grid ###
af_radio_auto.grid          (in_=af_frame, row=0, column=0,                 sticky=W        )
af_radio_manual.grid        (in_=af_frame, row=1, column=0,                 sticky=W        )
af_send_on_change.grid      (in_=af_frame, row=0, column=1,                 sticky=W        )
af_pos_scale.grid           (in_=af_frame, row=2, column=0,                 sticky=W+E+N+S  )
af_pos_send_on_change.grid  (in_=af_frame, row=2, column=1,                 sticky=W+S      )
af_send_button.grid         (in_=af_frame, row=3, column=0, columnspan=2,   sticky=W+E+N+S  )

af_frame.columnconfigure(0, weight=1)

af_frame.rowconfigure(0, weight=0)
af_frame.rowconfigure(1, weight=0)
af_frame.rowconfigure(2, weight=0)
af_frame.rowconfigure(3, weight=0)

###########################################################
### TOP ###################################################
###########################################################

### Grid ###
ae_frame.grid(in_=top, row=0, column=0, sticky=W+E)
af_frame.grid(in_=top, row=1, column=0, sticky=W+E)
top.columnconfigure(0, weight=1)
top.rowconfigure(0, weight=0)
top.rowconfigure(1, weight=0)

mainloop()

