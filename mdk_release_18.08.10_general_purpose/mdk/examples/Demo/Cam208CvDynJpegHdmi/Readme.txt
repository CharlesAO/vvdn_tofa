Cam208CvDynJpegHdmi

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Stream one imx208 camera to HDMI output and save the image to SD card when the B1 button is pressed.

Software description
=======================

This application uses the Leon code and a CV pipeline running on shaves. Basically, these are the few steps made during the application:
    1. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    2. Configure the camera sensor, the in chip datapath (MIPI, CIF/SIPP,LCD components), the out of chip datapath (HDMI external chip).
    3. Run CV pipe on frames

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.
A MV200 board (containing one or two imx208 sensors) connected to the CAMB connector of the MV182 board
An HDMI cable connected to a monitor.
A SD card formated in FAT32.
The SD card needs to have min 2MB free space.

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

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter. Code section is larger on ma2150 ma2450

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
On debug console there are listed the configuration steps performed
If the card is not inserted, the error message: 
	LeonRT (P0:ALRT) execution interrupted temporarily.
    Shave 5 (P0:AS5) execution interrupted temporarily.
    Shave 6 (P0:AS6) execution interrupted temporarily.
    Shave 7 (P0:AS7) execution interrupted temporarily.
    Shave 8 (P0:AS8) execution interrupted temporarily.
    Shave 9 (P0:AS9) execution interrupted temporarily.
    Shave 10 (P0:AS10) execution interrupted temporarily.
    Shave 11 (P0:AS11) execution interrupted temporarily.
    LOS: LeonOS (P0:ALOS) suspended at 0x80001110 (Application terminated successfully)
    PIPE:LOS: 
    PIPE:LOS: ERROR: SDIO was not succesfully initialised
    PIPE:LOS: SD Card not found
    PIPE:LOS: Exited with error code 27

will be printed on debug console.

If the card is inserted, the message: 
	OsDrvSdioInit sc RTEMS_SUCCESSFUL
	rtems_bdpart_register_from_disk sc RTEMS_SUCCESSFUL
will be printed on debug console.

The displayed video will be 1920 x 1080p, at a frame rate of 30 fps.

User interaction
==================

When the B1 button is pressed, the currently displayed frame will be copied in a separate buffer which will be converted to JPEG and then stored on SD card. 
Filenames are automatically indexed to prevent overwriting an already existing filename.
