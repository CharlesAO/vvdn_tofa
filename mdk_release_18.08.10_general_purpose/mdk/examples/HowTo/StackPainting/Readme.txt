StackPainter

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
This is a simple SHAVE stack painter example.

Software description
=======================
This example demonstrate checking of SHAVE stack area (without debugger support) as for this purpose performs the following operations:
     - painting of preselected stack area with canary values
     - simulate stack usage (with test pattern) of multiple SHAVES.
     - dump of stack area (output result) into file or UART.

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
The default Myriad revision in this project is ma2150 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type make "start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2150 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
DEBUG: unitTestMemCompare()   : (7001FFCC vs 701A091C l:00000028)             => PASS
DEBUG: dumpMemoryToFile()     : (7001FF60 l:00000094)[shv0_stack.bin]

DEBUG: unitTestMemCompare()   : (7003FEF4 vs 701A0D1C l:00000100)             => PASS
DEBUG: dumpMemoryToFile()     : (7003FE88 l:0000016C)[shv1_stack.bin]

DEBUG: unitTestMemCompare()   : (7005FF7C vs 701A111C l:00000078)             => PASS
DEBUG: dumpMemoryToFile()     : (7005FEF8 l:000000FC)[shv2_stack.bin]

DEBUG:
DEBUG: moviUnitTest:PASSED

DEBUG:
LOS: LeonOS (P0:ALOS) suspended at 0x7019032C (Application terminated successfully)
PIPE:LOS: TEST ON SHAVE 0
PIPE:LOS: Unused stack before processing:36 W
PIPE:LOS:
PIPE:LOS: Saving stack SHAVE0 into file shv0_stack.bin
PIPE:LOS: Unused stack after processing:26 W
PIPE:LOS:
PIPE:LOS:
PIPE:LOS: TEST ON SHAVE 1
PIPE:LOS: Unused stack before processing:90 W
PIPE:LOS:
PIPE:LOS: Saving stack SHAVE1 into file shv1_stack.bin
PIPE:LOS: Unused stack after processing:26 W
PIPE:LOS:
PIPE:LOS:
PIPE:LOS: TEST ON SHAVE 2
PIPE:LOS: Unused stack before processing:62 W
PIPE:LOS:
PIPE:LOS: Saving stack SHAVE2 into file shv2_stack.bin
PIPE:LOS: Unused stack after processing:32 W
PIPE:LOS:
PIPE:LOS:



