012_SharingData

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
This example uses shared data between processors. For a better understanding of the configuration, please read the Programmer's guide.

Software description
=======================

This application uses both Leon and Shave code. Basically, these are the few steps made during the application:
    1. Configure the sistem (clocks and memory).
    2. Setup expected buffers
    3. Start the other leon and the other shaves:
Since this application implies loading shave applications dynamically, we have to set it properly:
    3.1. Setup the SHAVEs that will be used to execute the shave code
    3.2 Open those shaves
    3.2 Run shave applications on target shaves. The desired message will be printed.
    3.4 Wait for for SHAVEs to finish
    3.5 Close those shaves
    3.5 Free the memory previously used. 
    4. Check the validity if the data

The shave code has two modules: one managing small data and the other for large data. 

In the first one, we used uncached memory, and in the second, which implies sharing big data with Leon we have used DDR. 
In thi last case, the CMX-DMA engine was used for managind data on shave side, and the operation o
n caches for the leon side on receiving a valid data.
For the LOS-LRT sharing was choosen the cmx direct data.

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

Where {Myriad_version} may be ma2150 or ma2450;
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

Where {Myriad_version} may be ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
 The result consist in printf seen using the debugger. It looks like the following:

PIPE:LOS: Share small data between leon and shave 
DEBUG: unitTestMemCompare()   : (78200CB8 vs 78200DA8 l:00000078)             => PASS
PIPE:LOS: Write small data from two SHAVEs 
DEBUG: unitTestMemCompare()   : (78200C40 vs 78200D30 l:00000078)             => PASS
PIPE:LOS: Share data between leons
DEBUG: unitTestCompare()      : (0xAABBCCDD,0xAABBCCDD)                       => PASS
PIPE:LOS: Share big data between leon and SHAVEs 
PIPE:LOS: 
PIPE:LOS: ---- Loading ImageFile ----
DEBUG: loadMemFromFile()      : o:000000 l:064000 -> 0x8000FA00 [../../../../common/../resources/CobKitchen_1280x720.yuv] 
PIPE:LOS: Srite big data from two shaves
DEBUG: unitTestMemCompare()   : (80000000 vs 8000FA00 l:0000FA00)             => PASS
DEBUG: 
DEBUG: moviUnitTest:PASSED

DEBUG: 

