LeonToLeonMessage

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This test demonstrates the use of Leon IPC calls.

Software description
=======================
Application which sets up communication between Leon cores using the standard paradigm:
LeonOS running RTEMS
LeonRT running bare-metal

Hardware needed
==================
Runs on any Myriad2 HW platform. Does not contain board specific settings.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The results  consist in printf seen using the debugger:
UART: LeonOS Started.
UART: LeonRT Start Addr: 0x701C1000

Afterwards, the application allows for sending up to 5 messages from LOS to LRT
This is done by using the moviDebug command prompt and issuing:
mset LrtSendMessage 1

for up to 5 times

The expected output is:
UART: LRT: Cofee received from LOS: 0xCAFE000x

Where x is the number of the message
