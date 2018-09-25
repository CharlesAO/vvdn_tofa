LowPower

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150/ma2450 silicon

Overview
==========
This example is an RTEMS application that uses the Power Manager component 
to configure and enter into Low Power mode.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 boards

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

To build the project please type:
Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}""

Where {Myriad_version} may be ma2150 (default) or ma2450

Setup
==================
To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 (default) or ma2450.

Expected output
==================
The debugger window will print debugging messages about the ongoing low power actions:
"
UART:  *** Test entering low power with a multitasking application
UART:  T
UART: ask0 started
UART:  Task1 started
UART:  Task2 started
UART: called cbRequest
UART: called cbRequest
UART: called cbRequest
UART: called cbCancel
UART: called cbRequest
UART: called cbRequest
UART: called cbRequest
UART: called cbCancel
UART: called cbCancel
UART: called cbRequest
UART: called cbRequest
UART: called cbRequest
UART: called cbGranted
UART: called cbGranted
UART: called cbGranted
UART: 
UART:  The system will now enter into the configured low power mode;
UART:  to wakeup the system, set the DIP SWITCH8 to ON (UP position)
UART: 
UART: called cbExit
UART: called cbExit
UART: called cbExit
UART:  Task0 finished
UART:  Task1 finished
UART:  Task2 finished
UART: 
UART:  Test finished
"

User interaction
==================
When the debugger message shows "to wakeup the system, set the DIP SWITCH8 to ON (UP position)",
please act accordingly. The messages will hang until the required action is done
