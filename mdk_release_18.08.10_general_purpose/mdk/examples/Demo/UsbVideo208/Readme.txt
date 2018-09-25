UsbVideo208

Supported Platform
===================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Send the stream data from imx208 camera through USB bus.
This example may also be used for profiling DDR vs CMX pipeline usage. The tests done on this example showed:
SIPP in CMX (LUT on 1080p image), CMX usage is around 42 KB:
P0:AA% mget cyclesSIPP -type int
ADDRESS     NAME        VALUE    SIZE  TYPE_NAME  CONTEXT_ID
0x82020328  cyclesSIPP  8627493     8  <int>      P0:AA     
P0:AA% mget sippTimePassed
ADDRESS     NAME            VALUE               SIZE  TYPE_NAME       CONTEXT_ID
0x82020320  sippTimePassed  17.905885696411133     4  float volatile  P0:AA   

SIPP in DDR (LUT on 1080p image): (DDR_SIPP_DATA enabled and DDR_SIPP_TRICKLE_DATA enabled), CMX usage is around 200 bytes
P0:AA% mget cyclesSIPP -type int
ADDRESS     NAME        VALUE    SIZE  TYPE_NAME  CONTEXT_ID
0x82020328  cyclesSIPP  9567775     8  <int>      P0:AA     
P0:AA% mget sippTimePassed
ADDRESS     NAME            VALUE               SIZE  TYPE_NAME       CONTEXT_ID
0x82020320  sippTimePassed  19.878372192382812     4  float volatile  P0:AA   

10% penalty running from DDR
around 0.45 cycle per pixel overhead

SIPP in DDR (LUT on 1080p image): (DDR_SIPP_DATA enabled), CMX usage is around 4 KB
P0:AA% mget cyclesSIPP -type int
ADDRESS     NAME        VALUE    SIZE  TYPE_NAME  CONTEXT_ID
0x82020328  cyclesSIPP  8828588     8  <int>      P0:AA     
P0:AA% mget sippTimePassed 
ADDRESS     NAME            VALUE               SIZE  TYPE_NAME       CONTEXT_ID
0x82020320  sippTimePassed  18.387216567993164     4  float volatile  P0:AA 
3.7% penalty running only the scheduler in DDR


Software description
=======================

This application uses the Leon code and a SIPP pipeline running on shaves. Basically, these are the few steps made during the application:
    1. Start the USB DataPump on LeonOS
    2. General configurations of the board (internal clocks, external clock generator for sensors, GPIOs, ...)
    3. Configure the camera sensors and the in chip datapath (MIPI, SIPP)
The application works with both USB 3 and USB 2 hosts.

Hardware needed
==================
A MV182 or MV212 board, housing the Myriad2 chip.
An USB cable connection from MV182 or MV212 to a PC host.

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
Myriad2 silicon - To run the application - connect an USB cable from mv182 board to the a PC
The application works for USB 2 and USB 3, depending on the used cable type/USB port.

Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Expected output
==================
The debug console should show the following messages:

PIPE:LOS:
PIPE:LRT: Configuring the SIPP pipeline
PIPE:LOS:
PIPE:LRT: Configuring imx208 camera and datapath
PIPE:LOS: UsbPump_Rtems_DataPump_Startup()!
PIPE:LOS:

The displayed video will have a resolution of 1920 x 1080.
When the application starts, the PC host should indentify the camera and the USB device speed,
displaying one of the following message:

PIPE:LRT: Camera initialized in HighSpeed mode
or
PIPE:LRT: Camera initialized in SuperSpeed mode


Depending on the USB speed the framerate of the video is different: 60fps for SuperSpeed and
around 11fps for High Speed.

The video streaming can be captured using a stream capture application.
Example of stream viewers:
    Windows: amcap, vlc, skype
    Linux: cheese, guvcview, xawtv, skype

Note: some of the applications above may show some frame tearing when the video is displayed.
The recomended applications in order to avoid this are: vlc for Windows and luvcview for Linux.

User interaction
==================
