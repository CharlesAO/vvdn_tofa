MultiStream

Overview
====================
This app illustrates concurrent operation of multiple pipelines running within the SIPP framework
in direct streaming runtime.

Hardware needed
==================
Myriad2 - This example works on Myriad2: ma2450 silicon

Build
====================
Change directory to the required build platform.

build/myriad2 (mv0212 or mv0182 board): To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} is ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter. Code section is larger on ma2450

Setup
====================
MV0182
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} is ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Notes
====================
This is a LeonRT demonstration example. It can not be run from within Eclipse and will not work if flashed
into EEPROM unless example is changed to run on LeonOS and interrupts are re-routed to LeonOS.

Expected output
====================
The output in this case is a console listing of the frames of each pipe as they progress.
Pipeline 1 has been deliberately chosen to be significantly less complex than pipeline 0 so that it may be observed
that while pipeline 0 is started first, pipeline 1 will overtake it and complete several frames while pipeline 0 is
operating on its first frame to demonstrate concurrent operation.

When it's finished running you will get two output files in the folder:

myriad2: outA_512x128_P420.yuv
         outB_512x512.raw

Open the images with Irfanview, pyuv or movImageCompare. The files have the following format:
outA_512x128_P420.yuv: - width:  512
                       - height: 128
                       - 24bpp (3 8bit plane)
outB_512x512.raw :     - width:  512
                       - height: 512
                       - 8bpp (1 8bit plane)

