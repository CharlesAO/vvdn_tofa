Cam378OpipeHdmi

Supported Platform
==================
Myriad2 - This example works on Myriad2, ma2450 silicon

Overview
==========
Stream one IMX378 camera-> MipiRx -> Opipe -> HDMI
No dynamic sensor tuning and no 3A(autofocus, autoexposure or autowhite balance).
Fixed tuning data for indoor light conditions:
		- TL84: Fluorescent
		- D65:  Daylight
		- A:    Incandescent
Currently TL84 is set.
For more information on the Myriad2 Image Signal Processing block (Opipe), please read the relevant chapter in the Myriad2 databook

Software description
=======================
This application runs on the Leon RT. Basically, these are the few steps made during the application:
    1. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    2. Configure the camera sensor, the in chip datapath (MIPI, CIF/SIPP,LCD components), the out of chip datapath (HDMI external chip).
    3. Configure the OpipeRx (MipiRx->DDR)
    4. Configure the OpipeMF (main or full pipe data struct)

Hardware needed
==================
A MV212 board, housing the Myriad2 ma2450 chip.
A MV249 board (containing one imx378 sensor) connected to the CAMA connector of the MV212 board
An HDMI cable connected to a monitor.

Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 silicon - To build the application:
     - "make clean"
     - "make all"

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type make "start_server"
    - open another terminal and type "make run"

Expected output
==================
On debug console there are listed the configuration steps performed
The displayed video will be 1920 x 1080p, at a frame rate of 30 fps
