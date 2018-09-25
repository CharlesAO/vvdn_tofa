testHwConvUpperSlices

Overview
====================
This is a simple SIPP pipeline to test the hardware convolution block on upper slices

Hardware needed
==================
MV0182 or MV212 board - This example works on ma2150 and ma2450 silicon

Build
====================
Change directory to the required build platform.

build/myriad2 (mv0212 or mv0182 board): To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
====================
MV0182 or MV0212
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
====================
1. Expected displayed message: "moviUnitTest:PASSED"
2. Expected output file: the output generated is written to a file:
out_1280x4_HwConv.raw. This is not a viewable image.

