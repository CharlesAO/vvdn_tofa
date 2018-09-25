HelloWorldShave

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator
Note: this application executes many cycles as such running it on moviSim will take several tens of minutes
depending on your system


Overview
==========
The application prints the cyclecount in different stages of implementing a code in assembly language.
The application takes the user through the steps recommended to perform assembly level optimizations.
This is done using an example convolution of 1x7 over a 320x240 grayscale-only (luma-only) input image.

Software description
=======================

This application uses both Leon and Shave code. Basically, these are the few steps made during the application:
    1. Configure the sistem.
    Since this application implies the shave0 using, we have to set it properly:
    2. Reset shave 0
    3. Set the absolute default stack;
    4. Start shave 0
    5. Wait  shave
    6. On this step, the setup is complete. The desired message can now be printed.

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
 The result consist in printf seen using the debugger.
UART: AsmOptimisation algorithm.
UART:
UART: Step 0:  No optimisation - C level
UART:
UART: Shave measured instruction count: 4027077
UART:
UART: Step 1:  First version of code written in Asm
UART:
UART: Shave measured instruction count: 609259
UART:
UART: Step 2:  Remove unnecesary "nop's" and start putting instructions in parallel - Asm
UART:
UART: Shave measured instruction count: 513042
UART:
UART: Step 3:  Loop parallelization - Asm
UART:
UART: Shave measured instruction count: 493838
UART:
UART: Step 4:  First instance of written Asm
UART:
UART: Shave measured instruction count: 316371
UART:
UART: moviUnitTest:PASSED



