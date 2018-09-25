UsbHidMouse

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This example uses Myriad as a mouse device to send some cursor moving reports
to the host.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 boards.


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
Connect an USB cable from mv182 board to the a PC
The application works for USB 2 and USB 3, depending on the used cable type/USB port.

Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
    - the mouse cursor on the host computer should move for a small distance
    to the upper left corner of the screen

User interaction
==================
