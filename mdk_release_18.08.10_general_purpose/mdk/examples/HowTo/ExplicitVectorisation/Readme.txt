ExplicitVectorisation with RTEMS

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Example to use explicit vectorization.

Software description
=======================

This application uses both the Leon and the Shave code.
Basically, these are the few steps made during the application:
    1. The Clock Power reset module is initialized.
    2. The input parameters SHAVEs are set.
    3. the shave is started.
    4. The final result is computed.
    Basically, the user makes a simple add operation using shave vectorization and checks the final result.
    This is a typical example on how to use shave vectorization.
    The file moviVectorUtils.h has been used for an easy access to vector types. After declaring pointers to integer vector, a simple vector addition was enough to prove the concept.

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
 The results  consist in printf's seen using the debugger.
 There addition done in the code is verified at the and and printed

 "Explicit Vector Add passed"


