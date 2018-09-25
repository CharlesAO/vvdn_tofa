CamSIPPDebayerHdmi

A SIPP project template example running from camera input
=========================================================

Supported Platform
===================
This example works on Myriad2: ma2150, ma2450 silicon.
PC and Linux - This example works on Linux gcc

Overview
==========
This is a simple SIPP pipeline example built with the SIPP Designer, that takes a raw image from the input and directs it to the output.
For exemplification purposes, negative was used in he pipeline to create a mix of HW and SW filters.
The effect shown is one similar to an Emboss video effect with chroma planes set on negative.

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.
A MV201 board (containing one imx214 sensor) connected to the CAMA connector of the MV182 or MV212 board or a MV249 board (containing one imx378 sensor) connected to the CAMA connector
An HDMI cable connected to a monitor.


Build
==================
Change directory to the required build platform.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 hardware: "make clean all MV_SOC_REV={Myriad_version}"
PC linux (or cygwin): Run "make clean all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
MV0212 or MV0182 boards (Myriad2 hardware)
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
Image with Embossed effect on the screen

If using adding new SW filters to this example, the pipeline needs to be configured on any number of SHAVEs of 0,1,2,3,4,5 and 6 but other SHAVEs cannot be enabled unless the corresponding memory sections are changed in the application linker script.
