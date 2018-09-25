Stereo208SDCard

Supported Platform
===================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon.

Overview
==========
Stream two imx208 cameras to a single mixed image on HDMI output + and saves the 2 separate streams to SDCard as leftCam.yuv and rightCam.yuv.


Software description
=======================

This application uses the Leon code and a SIPP pipeline running on shaves. Basically, these are the few steps made during the application:
    1. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    2. Configure the camera sensors, the in chip datapath (MIPI, CIF/SIPP,LCD components), the out of chip datapath (HDMI external chip).

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.
A MV200 board (containing 2 imx208 sensors) connected to the CAMB connector of the MV182 or MV212 board
An HDMI cable connected to a monitor. - Not mandatory
A propely formatted* Class 10 Sdcard

*properly formatted Sdcard can be obtained by:
1. On Linux using the formatProper.sh shell script supplied by mdk/common/utils
2. On Windows one can use the https://www.sdcard.org/downloads/formatter_4/.

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

Expected output
==================
On debug console there are listed the configuration steps performed
HDMI: The displayed video will be 1920 x 1080p (composed by two concatenated 960 x 1080 video obtained from the sensors), at a frame rate of 30 fps
SDCard: 2 files (leftCam.yuv and rightCam.yuv., one for each camera) will appear on the SDCard, containing an async stream. (500 Frames each)

After saving the predefined number of 500 frames (per camera) to the SDCard, it will display speed statistics as Average Write speed and minimum write speed.
Streaming to the LCD connected with HDMI will continue.

User interaction
==================


