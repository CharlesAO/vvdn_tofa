SippTutA3

THIS IS A SIMPLIFIED IMAGE PROCESSING PIPLEINE.
IT IS SUPPLIED FOR DEMONSTRATION PURPOSES ONLY.


Supported Platform
===================
Myriad2 - This example works on MV0182 or MV212 board.

Overview
==========
This is a simple SIPP pipeline example, built with the SIPP Designer, to test the 5x5 convolution block.
The input and output images buffer are located in DDR.

Hardware needed
==================
MV0182 or MV212 development board

Build
==================
Change directory to the required build platform.

build/myriad2:

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Expected output
==================
1. Expected displayed message: "moviUnitTest:PASSED"
2. Expected output file: An image with emphasized edges is dumped to a file,
View with movImageCompare, Irfanview or pyuv; The resolution is 512x512, YUV, P400
See mdk/resources/pics for sample images
