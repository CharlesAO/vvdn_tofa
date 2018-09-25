Readme.txt

mtrr_rtmes

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
LeonOS running RTEMS with two functions that are handled by MTRR.

Software description
=======================
This application adds two function footprints in memory type range register, and provides way to change memory configuration.

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
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The application output should be similar to:

UART:
UART: RTEMS POSIX Started
UART: Calculated sizes in bytes: 28 28
UART: Thread 1 created
UART: Thread 2 created
UART: val: 0x435822e1
UART: val: 0x4509dd84
UART: val: 0x435822e1
UART: val: 0x4509dd84
UART: val: 0x435822e1
UART: val: 0x4509dd84
UART: val: 0x435822e1
UART: val: 0x4509dd84
.............
