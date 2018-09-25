cppRtems_LosLrtShave

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and ma2150, ma2450 simulator.

Overview
==========
LeonOS running RTEMS starts with c++ build flow starts LeonRT which starts SHAVE

Software description
=======================
This application is a prove of concept for using c++ as development environment for LeonOS, LeonRT and SHAVEs.

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

UART: SHAVE Started.
PIPE:LOS: RTEMS POSIX Started
PIPE:LOS: Thread 1 created
PIPE:LOS: LeonRT Start Addr: 0x701C1000
PIPE:LOS: LeonRT Started.
PIPE:LOS: LeonRT Stop.
DEBUG: 
DEBUG: moviUnitTest:PASSED

DEBUG: 
LOS: LeonOS (P0:ALOS) suspended at 0x70181110 (Application terminated successfully)
