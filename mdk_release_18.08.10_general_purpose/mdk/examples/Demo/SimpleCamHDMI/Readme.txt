SimpleCamHDMI

Supported Platform
===================
Myriad2 - This example works on Myriad2 ma2150 and ma2450 silicon versions

Overview
==========
Stream one imx208 camera to HDMI output, using minimal configurations. The sensor information on 10 bits is cutdown to 8 bits and
a RAW8 -> YUV422P conversion is manually performed, by using the remaining RAW8 bits as luma plane and adding croma prefilled buffers with neutral information.
Further YUV422P -> YUV422I conversion is automatically performed in the LCD block, before outputing to HDMI

Software description
=======================
This application runs on the Leon OS. Basically, these are the few steps made during the application:
    1. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    2. Configure the camera sensor, the in chip datapath (MIPI, CIF/SIPP,LCD components), the out of chip datapath (HDMI external chip).

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.
A MV200 board, containing one imx208 sensor in the left position (J1), connected to the CAMB connector of the MV182 or MV212 board
An HDMI cable connected to a monitor.

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
The displayed video will be 1920 x 1080p, at a frame rate of 60 fps

User interaction
==================
none
