FramePump

Supported Platform
===================
Myriad2 - This test application works on Myriad ma2150/ma2450

Overview
==========
This application is showing:
- a part of the FramePump API: opening, configuring, starting FramePump for various sensors, as well as reading of frames
- the existence of frame tearing (based on frame counters in the additional frame metadata)

Software description
=======================
The application + FramePump runs on LOS (+ RTEMS).
The application will attempt to stream frames from any connected sensors on the 3 possible interfaces:
- 1 sensor connected to the CAMA interface
- 2 stereo sensors connected to the CAMB interface
The sensors each have their own selected configuration (resolution and frame rate),
selected in the brdInitializeSensors() function in brdRtems.c.

Hardware needed
==================
This software should run on one MV182/MV212 board with a camera module connected to either the CAMA or CAMB connector.
The supported cameras and modules are:
- CAMA: IMX208 (MV0200), 0V9282 (MV0250), OV7750(8cm baseline stereo)
- CAMB: IMX214 (MV0201), imx378 (MV0249)

Build
==================
To build the auto-test version
"make all"

Run
==================
To run the auto-test version
"make run"

Expected output
==================
I: [] runApp:137  Test finished
