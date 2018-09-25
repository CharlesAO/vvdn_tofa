JpegEncoderTest

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Convert an YUV image into JPEG

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
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
Two JPEG files should be written out to the test directory

UART: Starting JPEG Encode for ../../../resources/Baywatch_512x512_P422.yuv
UART: Loading Yuv file, please wait...
DEBUG: loadMemFromFile()      : o:000000 l:524288 -> 0x80000000 [../../../resources/Baywatch_512x512_P422.yuv]
UART: Encoded in: 1287821 cycles (2.682960 ms;   373 frames per second)
DEBUG: dumpMemoryToFile()     : (82400000 l:0000CEC3)[Baywatch.jpg]
UART: Encoded Image CRC32 OK [0x97845B95]
UART: Starting JPEG Encode for ../../../resources/CobKitchen_1280x720.yuv
UART: Loading Yuv file, please wait...
DEBUG: loadMemFromFile()      : o:000000 l:1382400 -> 0x80000000 [../../../resources/CobKitchen_1280x720.yuv]
UART: Encoded in: 2320592 cycles (4.834567 ms;   207 frames per second)
DEBUG: dumpMemoryToFile()     : (82400000 l:0001640A)[CobKitchen.jpg]
UART: Encoded Image CRC32 OK [0x42525464]
UART:
UART: moviUnitTest:PASSED

