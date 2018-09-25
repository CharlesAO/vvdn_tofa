MemRegionApi

Overview
====================
This is a simple SIPP pipeline to test the SIPP framework API sippAllocCmxMemRegion().

Hardware needed
====================
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
MV0182 or MV212
    - open terminal and type "make start_server"
    - open another terminal and type "make run"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
====================
When it's finished running you will get two output files in the folder:

myriad2: output0.raw
         output1.raw

Open the images with Irfanview, pyuv or movImageCompare. The files are .raw
image files with the following formats:
output0.raw:
    - width:  522
    - height: 128
    - 24bpp (3 8bit planes)
output1.raw:
    - width:  522
    - height: 128
    - 16bpp (1 16bit plane)

Test is self checking

