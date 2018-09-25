cppshave

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
LeonOS running with Bare metal and c++ starts SHAVE

Software description
=======================
This application integrates c++ singleton design pattern, for controlling SHAVEs

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

UART: Hello from LEON!
UART:
UART:
UART: Shave constructur called for id:0 at 0x70191618
UART:   Initialize Shave heap at 0x70008000
UART:   Global object constructed on Shave at 0x7000a91c
UART:   Stack object constructed on Shave at 0x7001ffd0
UART:     Hello from Shave's Global space
UART:     Hello from Shave's Stack space
UART:   Stack object destructed on Shave
UART:   Global object destructed on Shave
UART: Message from Shave's Stack!
UART: Shave destructor called for 0 at 0x70191618
