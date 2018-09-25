HelloWorldShave

Supported Platform
==================
This example works on Myriad2: ma2150, ma2450 silicon and ma2150, ma2450 simulator.

Overview
==========
Just print hello world using shave compiled code using stdio library

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
This is the easiest example on how to write a shave code. The application just prints hello world using shave compiled code.

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.

Build
=====
Please type "make help" if you want to learn available targets.
Before cross-compiling make sure you do a "make clean"!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.

Setup
==================
Myriad 2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"
Myriad 2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
 The result consist in printf seen using the debugger.
 Hello from SHAVE!


