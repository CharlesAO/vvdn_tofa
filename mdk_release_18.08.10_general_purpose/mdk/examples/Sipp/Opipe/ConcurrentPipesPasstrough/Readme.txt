ConcurrentPipesPasstrough

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The ConcurrentPipesPasstrough example uses 5 concurrent pipes, set in passtrough mode.
On the resources side, this application uses: LOS, LRT, Sipp HW, and CMX DMA.

The LOS is used just to start the relevant clocks and also the LRT which runs the opipe code. 
The CMX DMA driver has been used in the Opipe component.
This examples shows how to use 5 concurrent pipes. Each of them has only one filter. The filter used are: LSC, SIGMA, SHARPEN, CONV and UPFIRDN.
All the filters are configured in the passtrough mode.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 board.

Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 silicon - To build the application:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type make "start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Expected output
==================

DEBUG: loadMemFromFile()      : o:000000 l:131072 -> 0x700C0008 [lowLight_512x128.raw] 
DEBUG: loadMemFromFile()      : o:000000 l:131072 -> 0x700A0008 [lowLight2_512x128.raw] 
DEBUG: loadMemFromFile()      : o:000000 l:065536 -> 0x70050008 [lowLightU8a_512x128.raw] 
DEBUG: loadMemFromFile()      : o:000000 l:065536 -> 0x70030008 [lowLightU8b_512x128.raw] 
DEBUG: loadMemFromFile()      : o:000000 l:065536 -> 0x70010008 [lowLightU8c_512x128.raw] 
DEBUG: unitTestMemCompare()   : (700C0008 vs 70080008 l:00020000)             => PASS
DEBUG: unitTestMemCompare()   : (700A0008 vs 70060008 l:00020000)             => PASS
DEBUG: unitTestMemCompare()   : (70050008 vs 70040008 l:00010000)             => PASS
DEBUG: unitTestMemCompare()   : (70030008 vs 70020008 l:00010000)             => PASS
DEBUG: unitTestMemCompare()   : (70010008 vs 70000008 l:00010000)             => PASS
DEBUG: 
DEBUG: moviUnitTest:PASSED

