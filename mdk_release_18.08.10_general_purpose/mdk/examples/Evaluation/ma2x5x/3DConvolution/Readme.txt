3DConvolution

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Particular tensor implementation tailored for best performance for
3x3 convolutions applied over very small input size.

Software description
=======================
The application computes 3x3 convolutions over 64x56x56 (channels x
width x height) using 1 - 12 SHAVEs and generates 192 output maps.
It checks correctness and reports pass / fail.
The run time is shown for each SHAVEs combination.

Hardware needed
==================
Myriad2 -This software should run on MV182, MV212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

To run the project: "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter. Code section is larger on ma2150 ma2450

Setup
==================

* For MDK General Software Package
==================================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

* For MDK Evaluation Package
=================================
Windows:
Starting the server:
in one Command Prompt, from the root directory of the release, go to the mdk/examples/Evaluation/ma2x5x path then type startDebugServer.bat
Running the application:
in another Command Prompt, from the previously mentioned path type runApp.bat <AppName> 
or
go to the application path and type run.bat

Linux:
Starting the server:
in one Terminal, from the root directory of the release, go to the mdk\examples\Evaluation\ma2x5x path then type bash startDebugServer.sh
Running the application:
in another Terminal, from the previously mentioned path type bash runApp.bat <AppName> 
or
go to the application name to be run and type bash run.sh

Expected output
=================
PIPE:LOS: 3D Convolution benchmark: conv3x3 on 64x192x56x56
PIPE:LOS: System frequency: 600 Mhz
PIPE:LOS: 
P0:ALOS% PIPE:LOS: SHAVEs  1: 117224510 cycles, 195.37 ms,  5 fps,  3.47 gflops 
PIPE:LOS: SHAVEs  2:  58680196 cycles,  97.80 ms, 10 fps,  6.94 gflops 
PIPE:LOS: SHAVEs  3:  39076059 cycles,  65.13 ms, 15 fps, 10.40 gflops 
PIPE:LOS: SHAVEs  4:  29315213 cycles,  48.86 ms, 20 fps, 13.87 gflops 
PIPE:LOS: SHAVEs  5:  23844667 cycles,  39.74 ms, 25 fps, 17.34 gflops 
PIPE:LOS: SHAVEs  6:  19583059 cycles,  32.64 ms, 30 fps, 20.81 gflops 
PIPE:LOS: SHAVEs  7:  17144711 cycles,  28.57 ms, 34 fps, 23.58 gflops 
PIPE:LOS: SHAVEs  8:  14719542 cycles,  24.53 ms, 40 fps, 27.75 gflops 
PIPE:LOS: SHAVEs  9:  13570459 cycles,  22.62 ms, 44 fps, 30.52 gflops 
PIPE:LOS: SHAVEs 10:  12331783 cycles,  20.55 ms, 48 fps, 33.29 gflops 
PIPE:LOS: SHAVEs 11:  11187236 cycles,  18.65 ms, 53 fps, 36.76 gflops 
LOS: LeonOS (P0:ALOS) suspended at 0x7019032C (Application terminated successfully)
PIPE:LOS: SHAVEs 12:   9917997 cycles,  16.53 ms, 60 fps, 41.62 gflops 
PIPE:LOS: 
PIPE:LOS: moviUnitTest:PASSED
