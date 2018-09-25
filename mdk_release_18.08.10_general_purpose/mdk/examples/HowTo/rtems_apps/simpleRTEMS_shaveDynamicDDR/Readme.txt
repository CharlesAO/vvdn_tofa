simpleRTEMS_shaveDynamic

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
LeonOS running RTEMS loads and starts SHAVE binary dynamically.

Software description
=======================
This application starts two RTEMS threads with same thread function and start SHAVE entry function

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
The application output should be similar to:

PIPE:LOS:
PIPE:LOS: RTEMS POSIX Started
PIPE:LOS: Add thread created
PIPE:LOS: Sub thread created
PIPE:LOS: S0 Data set 1 at address 0x80010030 : 4 25 313 13
PIPE:LOS: S0 Data set 2 at address 0x80010040 : 4 30 400 500
PIPE:LOS: S0 Start at addr:0x1D000010
PIPE:LOS: S1 Data set 1 at address 0x80030030 : 4 25 313 13
PIPE:LOS: S1 Data set 2 at address 0x80030040 : 4 30 400 500
PIPE:LOS: S1 Start at addr:0x1D000010
PIPE:LOS: S 0 res from address 0x80010050: 8 55 713 513
PIPE:LOS: S 1 res from address 0x80030050: 8 55 713 513
PIPE:LOS: S2 Data set 1 at address 0x80050030 : 4 25 313 13
PIPE:LOS: S2 Data set 2 at address 0x80050040 : 4 30 400 500
PIPE:LOS: S2 Start at addr:0x1D000010
PIPE:LOS: S3 Data set 1 at address 0x80070030 : 4 25 313 13
PIPE:LOS: S3 Data set 2 at address 0x80070040 : 4 30 400 500
PIPE:LOS: S3 Start at addr:0x1D000010
PIPE:LOS: S 2 res from address 0x80050050: 0 -5 -87 -487
PIPE:LOS: S 3 res from address 0x80070050: 0 -5 -87 -487
DEBUG:
DEBUG: moviUnitTest:PASSED

DEBUG:
LOS: LeonOS (P0:ALOS) suspended at 0x70185110 (Application terminated successfully)
