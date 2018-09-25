DmaProcessingOptimized

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Loads input image in DDR and applies Darkness effect on it.

Software description
=======================

This application uses both the Leon and the Shave code. Basically, these are the few steps made during the application:
    1. Initial setup.
    2. Run the algorythm on shave.
    3. Determine the cylces number.

Basically, this application loads a data in DDR. The output image is obtained by applying the darkness on the y plane
and copy the other two components(U and V).

In applying the darkness on the Y component, a simple optimization is made in bringing and processing the input lines.
A new set of data is being brought meanwhile  the previous one is send as input to the Darkeness function.
This optimize method is not made on sending the output lines.

In copying both planes(U and V), no optimization has been made. The task is set and started. Another process it will be taken into account only after the previous one finises.
This applies both to input and output lines.

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
The results  consist in printf seen using the debugger.
Input image and output image can be verified with debugger, only when running with "make debug MV_SOC_REV={Myriad_version}"
Save the output by typing:
mdbg::savefile image_80x60_P420.yuv outputFrame 7200
