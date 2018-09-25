DynamicInfrastructure_C

Supported Platform
===================
 Myriad2 - This example works on ma2150 and ma2450 Myriad2 simulator and on
ma2150 and ma2450 Myriad2 silicon .

Overview
==========
LeonOS running RTEMS loads SHAVE application data section and SHAVE library group data section
and starts shaves.

Software description
=======================
This application shows a relativel fully features SHAVE running example with data dynamicall loaded

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 board.

Build
==================
Please type "make help" if you want to learn available targets.

To build the project please type:
"make clean"
"make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450
so it is not necessary to specify the MV_SOC_REV in the terminal.

NOTE: Due to the larger code size, this app cannot
build in debug mode, therefore an additional flag
has been added to ingore the MV_BUILD_CONFIG=debug parameter.

Setup
==================
Myriad2 simulator - To run the application:
  - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
  - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
  - open terminal and type "make start_server"
  - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450
so it is not necessary to specify the MV_SOC_REV in the terminal.

Expected output
==================
The application will keep showing messages coming in various forms
from the three stressed shaves
