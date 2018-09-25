WarpJpegDyn

Supported Platform
===================
Myriad2 - This example works on Myriad2 ma2150 and ma2450 silicon versions

Overview
==========
Stream one imx208 camera to HDMI output, at the resolution of 1080p and the frame rate is 15 fps, using 4 different types of warp effects (identity, fisheye, wave, and perspective).
Each time the B1 button is pushed, shave dynamic loading is used to load a JPEG encoding SHAVE app, a JPEG capture is made and it is shaved on the SDCard.

Software description
=======================
This application runs on the Leon OS(where RTEMS and the SDCard is implemented) and Leon RT(where warp is running using a shave dynamic component). Basically, these are the steps made during the application:
    1. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    2. Configure the camera sensor, the in chip datapath (MIPI, CIF/SIPP,LCD components), the out of chip datapath (HDMI external chip).
    3. Set up the messhes needed for identity, fisheye, wave, and perspective
    4. Run for (FRAMES_PER_WARP / 15) seconds each warp
     
Hardware needed
==================
A MV182 board, housing a ma2150/ma2450 Myriad2 chip.
A MV200 board, containing one imx208 sensor in the left position (J1), connected to the CAMB connector of the MV182 board
An HDMI cable connected to a monitor.
A SDcard placed in the SDCard socket. The example works without it but cannot do the JPEG saving on the SDCard.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

You have to be aware of the Myriad2 {Myriad_version} of your MV182 or MV0212 board and use it accordingly in the following build/run commands.

To build the project please type:
"make clean"
"make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 - To run the application:
    - open terminal and type "make start_server MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The displayed video will be 1920 x 1080p, at a frame rate of 15 fps. Each warp effect will last (FRAMES_PER_WARP / 15) seconds.
JPEG images are saved on the SDCard by pushing the B1 button. This can be one of the four efects produced by the warp.

User interaction
==================
B1 button on the MV212 or MV128 board (top left corner of the board) triggers saving of JPEG picture on the SDCard.
