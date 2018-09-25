simpleRTEMS_shave

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
This example applies two effects to an image. For the upper half of the image
the darken effect is used and for the lower half a brightness effect. Each effect
is run on a different shave.
Each shave is started from a thread. Once the shave is started a semaphore is used
to block the thread until the shave execution is done. The interrupt handler releases
the semaphore and puts the thread into ready state.

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.

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
Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
UART:
UART: RTEMS POSIX Started
UART: Thread 1 created
UART: Function 1
UART: Thread 2 created
UART: Function 2


User interaction
==================
Input and output images can be verified with the debugger, only when running with "make debug MV_SOC_REV={Myriad_version}"
Save the output by typing:
mdbg::savefile image_80x60_P420.yuv outputFrame 7200

