GtImageWarp

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Loads input image in DDR and applies the fish_eye correction on an input image ( size = 320 x 240) using new component that has a new scheduler
wich will reduce data transfer between CMX and DDR.

Software description
=======================

This application uses both the Leon and the Shave code. Basically, these are the few steps made during the application:
    1. Initial setup.
    2. Run the algorythm on shave.
    3. Determine the cycles number.

Basically, this application loads a data in DDR. The output image is obtained by applying fish_eye algorithm on the y plane.

Build
==================
Please type "make help" if you want to learn available targets.

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

The expected output on simulator and silicon:

 - the shave number of cycles is printed into the terminal
UART: fish_eye algorithm.
UART:
UART: Shave measured instruction count: 659246
DEBUG: unitTestVerbosity()    : VERBOSITY_QUIET

DEBUG: unitTestCrcCheck()     : (addr:0x80100000,0x00012C00,0xF5F63D38)       => PASS
DEBUG:
DEBUG: moviUnitTest:PASSED

 - the output image will be created automatically


