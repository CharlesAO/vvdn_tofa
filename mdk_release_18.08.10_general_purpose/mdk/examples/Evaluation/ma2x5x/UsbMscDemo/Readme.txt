UsbMscDemo

Supported Platform
===================
Myriad2 - This example works on Myriad2 ma2150, ma2450 silicon.

Overview
==========
Mass storage class demo application that creates a 64MB RAM disk in DDR.

Hardware needed
==================
Myriad2 - This software should run on MV182, MV212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Setup
==================

* For MDK General Software Package
==================================
Myriad2 silicon - To run the application - connect an USB cable from mv182 board to the a PC.
The application works for USB 2 and USB 3, depending on the used cable type/USB port.

!!!Before cross-compiling make sure you do a "make clean"!!!

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
The following message is printed when the application starts:
    PIPE:LOS: USB Mass Storage Class demo started
If the device is connected in USB 3 port the following message should be displayed:
    PIPE:LOS: USB 3.0 storage device connected
    PIPE:LOS: The drive should now be visible on the host PC. Please format and perfor
    PIPE:LOS: m the speed benchmark (see the Manual for further details)
    PIPE:LOS: Storage size: 64.00 MB
    PIPE:LOS: Device disconnected
If the device is connected in USB 2 port the following message should be displayed:
    PIPE:LOS: USB 2.0 storage device connected.
    PIPE:LOS: The drive should now be visible on the host PC. Please format and perfor
    PIPE:LOS: m the speed benchmark (see the Manual for further details)
    PIPE:LOS: WARNING!!! This device is able to perform at higher speeds.
    PIPE:LOS: Please connect the board to an USB 3 host to achieve USB 3 performance.
    PIPE:LOS: Storage size: 64.00 MB
    PIPE:LOS: Device disconnected
If the device is detached from the host the following message should be displayed:
    PIPE:LOS: Device disconnected

User interaction
==================
Windows
    - check device detection on the Device Manager -> Disk Devices as
    “MCCI RAM-Disk-0 USB USB Device”
    - format the new detected storage device
    - at this point you have a formatted mass storage device  mounted into your file system,
      so you can create/copy/modify/delete files on it

    Steps needed to perform speed tests:
      1. Download and extract HD_Speed from the link above (no installation is needed)
      2. Start HD_Speed and select the needed operation (Read or Write)
      3. Optionally select the how much data to request for the OS in one transfer from the Block
      Size drop down list (see more details in HD_Speed FAQ)
      4. Press “Start” button to start the test

Linux
    gnome-disk-utility can be used to format and benchmark the device. If not already available,
    gnome-disk-utility can be installed on Ubuntu systems by running:
        sudo apt-get install gnome-disk-utility

    Steps needed to format the device perform speed tests:
      1. Start gnome-disk-utility by typing “gnome-disks” command on the command line
      2. From the dropdown menu select “Format” to format the disk
      3. After formatting press the “+” button to create a new partition. After this point the disk
      is ready to use.
      4. Select “Benchmark” from the dropdown menu.
      5. Press “Start Benchmark” button to start the speed test

    The current and average speed will be displayed for both read and writes along with a graph with
    the speed history

