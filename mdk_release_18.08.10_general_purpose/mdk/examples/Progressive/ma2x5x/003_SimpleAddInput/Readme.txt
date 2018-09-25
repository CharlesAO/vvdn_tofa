SimpleAddInput

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Just starts RTEMS, loads an input image and prints its first 8 bytes.


Software description
=======================
This application uses only the Leon.
Basically, these are the few steps made during the application:
    1. Configure the sistem
    2. An image[DunLoghaire_80x60.yuv] is imported and it is used as an object. The variable that manages frames is directed to that image.
    This is the simplest example on how to allocate a frame buffer.
    3. On this step, the setup is complete. The desired message can now be printed.


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
 The results consist in printf seen using the debugger.
 Input image can be verified with debugger, only when running with "make debug MV_SOC_REV={Myriad_version}"
 Save the input by typing:
 mdbg::savefile image_80x60_P420.yuv inputFrame 7200
