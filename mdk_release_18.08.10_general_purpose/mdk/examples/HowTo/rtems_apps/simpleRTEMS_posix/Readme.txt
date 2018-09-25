simpleRTEMS

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Starts two threads which run in parallel.
Each thread runs for a given time slice before releasing the processor for the other thread.

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
UART: C: 1
UART: C: 2
UART: C: 3
UART: C: 4
...
UART: C: 146
UART: C: 147
UART: C: 148
UART: D: 1
UART: D: 2
UART: D: 3
UART: D: 4
UART: D: 5
...
UART: D: 41
UART: D: 42
UART: D: 43
UART: D: 44
The ouput is similar with listing above (but the figures can vary between simulator run and board run). One of the thread prints D and the other C.
This shows that the thread switch is happening.


User interaction
==================
Each thread run in an infinite loop, so you should hit CTRL+C to close the application.

