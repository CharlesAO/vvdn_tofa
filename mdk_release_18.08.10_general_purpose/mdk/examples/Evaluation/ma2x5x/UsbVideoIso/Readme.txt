UsbVideoIso

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Video demo example streaming DDR placed images on USB using
isochronous transfers.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 board.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

To build the project please type:
Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}""

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================

* For MDK General Software Package
==================================

To run the application - connect an USB cable from MV212 or MV182 board to the a PC

Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

* For MDK Evaluation Package
=================================
Windows:
Starting the server:
in one Command Prompt, from the root directory of the release, go to the mdk/examples/Evaluation/ma2x5x path then type startDebugServer.bat
Running the application:
in another Command Prompt, from the previously mentioned path type runApp.bat <AppName>
or
go to the application path and type run.bat

Linux:
Starting the server:
in one Terminal, from the root directory of the release, go to the mdk\examples\Evaluation\ma2x5x path then type bash startDebugServer.sh
Running the application:
in another Terminal, from the previously mentioned path type bash runApp.bat <AppName>
or
go to the application name to be run and type bash run.sh

Expected output
==================
  The device speed can be modified at runtime by plugging it into an USB 2.0 or
  USB 3.0 port:
    PIPE:LOS: USB started
    PIPE:LOS:
    PIPE:LOS: USB 2.0 video device detected
    PIPE:LOS: Streaming resolution: 640x480
    PIPE:LOS: Expected frame rate 40fps
    PIPE:LOS: Device disconnected
    PIPE:LOS: USB 3.0 video device detected
    PIPE:LOS: Streaming resolution: 2104x1560
    PIPE:LOS: Expected frame rate 50fps

    When the device is plugged  into an USB 2 port:
    - 640x480 resolution images  streamed from DDR should be
    - the expected frame rate is @38fps
    - the connection speed will be printed:
        PIPE:LOS: USB detected speed: HighSpeed
    displayed with the stream viewer.
    When the device is plugged  into an USB 3 port:
    - 2104x1560 resolution images  streamed from DDR should be
    - the expected frame rate is @50fps
    - the connection speed will be printed:
        PIPE:LOS: USB detected speed: SuperSpeed
    displayed with the stream viewer.


User interaction
==================
    - wait for the device to be identified on the host
    - open a stream viewer to display video
        - stream viewers examples:
            Windows: amcap (http://noeld.com/programs.asp?cat=video), vlc
            Linux: cheese, guvcview, xawtv, luvcview, mplayer

