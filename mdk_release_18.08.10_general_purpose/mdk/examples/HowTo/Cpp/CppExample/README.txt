CppExamples

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
This is a simple Leon C++ barebones example. Just creates a few classes and destroys them.
limitations:
1) no STL library
2) no exceptions
3) global, statically constructed objects are supported only if you call call_global_constructors() at the start of main().
4) global, statically constructed object destructors will not be called
5) no delete operator
6) no RTTI

Software description
=======================
Some C++ classes are created and destroyed.

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


The message displayed should be:
UART: Constructing object on global2, at address 0x701d0320!
UART: Your name:Myriad2 String
UART: Hello Myriad2 String!
UART: Constructing object on stack, at address 0x701bfea0!
UART: Overridden function (obj on stack), obj @ 0x701bfea0
UART: Overridden function (obj on global), obj @ 0x701d0328
UART: Destroying object on stack, at address 0x701bfea0!
UART: After block where object is allocated on stack,
UART:     (destructor should have been called)
UART: Destroying object on global2, at address 0x701d0320!
UART: Destroying object on global, at address 0x701d0328!

