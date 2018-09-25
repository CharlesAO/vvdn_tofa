MonoPipe

Overview
====================
Basic demonstration of the suggested Mono Pipeline running within the SIPP framework
in direct streaming runtime.

Hardware needed
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Build
====================
Change directory to the required build platform.

build/myriad2 (mv0212 or mv0182 board): To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2150 so it is not necessary
to specify the MV_SOC_REV in the terminal.

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter. Code section is larger on ma2150 ma2450

Setup
====================
MV0182
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2150 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Notes
====================
This is a LeonRT demonstration example. It can not be run from within Eclipse and will not work if flashed
into EEPROM unless example is changed to run on LeonOS and interrupts are re-routed to LeonOS.

Expected output
====================
1. Expected displayed message: "moviUnitTest:PASSED"
2. Expected output file: An output image will be generated after run finishes
with the following characteristics:
- Name:            output_640x480.raw
- Location:        build/myriad2
- Format:          .raw
- Size(WxH):       640x480
- Bits per sample: 8
- No. of planes:   1
- Open with tools: movImageCompare, Irfanview or pyuv
