LamaBLISMatrixMatrixMultiply

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
The example multiply two 2048x2048 matrices stored in column format.
The two input matrices are generated using the PCmatrixGenerator.
Also PCmatrixGenerator multiply the 2 input matrix and generate the A*B matrix used to check the output of the Myriad app.
Please make sure that you run the PCmatrixGenerator before runing the Myriad app.
To run the application with smaller matrices first generate the A, B and A*B matrices using the PCmatrixGenerator and then change the SIZE in main.c

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
     - "make matrix"

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
UART:
UART: Start using 1 SHAVEs!
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG:
DEBUG: moviUnitTest:PASSED

DEBUG:
UART:
UART: Done in 3155024520 cycles






