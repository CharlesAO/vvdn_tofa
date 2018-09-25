USB host demo

Supported Platform
==================
Myriad2 - This example works on Myriad2 silicon.

Overview
==================
This example shows how to configure Myriad2 board to act as an USB host.

Software description
==================
The application configure and initializes USB DataPump stack to allow the host to service
multiple device classes. To achieve this it create device driver instances for each of the
supported class.
In the current implementation the supported classes are:
 - Mass Storage Class (MSC) - allows to connect USB mass storage devices to the host. The only
 supported format for mass storage devices is FAT32. The device must be formated to FAT32
 before being connected to the host
 - Generic Class - provides means to communicate with vendor specific devices that do not
 implement one of the standard classes

The application is configured to accept the connection of one or more devices. More devices
can be connected using a hub.

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.
Cables:
 - USB OTG cable (micro USB B male to USB A female)
 - a separate USB A male to USB A female with the following specification is needed to
connect USB 3 devices to the host:
    http://www.mouser.com/ds/2/445/692902100000-474774.pdf

Jumper J9 must be connected on the board for the host to work.

Telnet install for Cygwin
==================
In the cygwin install wizard, on the package selection window,
search for the packages that will install telnet named "inetutils" 
and select the "inetutils" packages from the "Net" category and 
change the "Net" option from "default" to "install". 

Build and run
==================
Please type "make help" if you want to learn available targets.
The project uses RTEMS shell.
To connect to the RTEMS shell open a terminal and type:
"make clean"
"make debug_telnet MV_SOC_REV={Myriad_version}"
where {Myriad_version} may be ma2150 or ma2450.
A message like the following should be displayed:
 Please open a telnet session on port 30010 for LOS console:
 telnet localhost 30010
Following the above instructions an RTEMS shell should be opened.

The mass storage device is automatically mounted and a speed test for reads
and writes is performed first time a device is connected.
The speed test assumes that a read.dat file exists on the device. If such a
file doesn't exist the speed test will not be performed, but the device can
still be accessed using RTEMS shell commands. The file should have at least
4MiB for the measurements to provide reliable results.

Expected output
==================
The following message should be displayed on opening RTEMS shell:
 RTEMS Shell on /dev/console. Use 'help' to list commands.
 [/] #

The following messages should be displayed on connecting a mass storage
device:

  fsmount: mounting of "/dev/usb_0_0" to "/mnt/usb_0_0" succeeded
  Open /mnt/usb_0_0/read.dat
  Start READ test...
   Read 135266304 bytes TimeTicks(1728) 76444 KB/sec

  Start WRITE test...
   Write 134217728 bytes TimeTicks(1472) 89042 KB/sec

  End File_System_Performance_Thread

Known issues
==================
Due to high inrush current, some USB devices may trigger the MV0182/MV0212
over current protection circuit causing the board to turn off.

