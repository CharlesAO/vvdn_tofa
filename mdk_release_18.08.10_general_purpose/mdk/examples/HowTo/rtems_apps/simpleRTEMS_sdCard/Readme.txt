simpleRTEMS_sdCard

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The example initializes the SDIO Driver in Posix_Init and then creates and starts a thread that mounts a
filesystem, creates a file, writes 5 MB data to the file, synchronizes, reads back 5 MB, verifies data, and finally
unmounts the filesystem.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 boards.
SDHC Cards - V2.0 onwards (High Speed supported). Cards must be formatted FAT32, maximum 32K Clusters for maximum performance.

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
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"


Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================

UART:
UART: RTEMS POSIX Started
UART:
UART: osBoard0182Initialise 0
UART:
UART: Sdio driver initialising
UART:
UART: OsDrvSdioInit sc RTEMS_SUCCESSFUL
UART:
UART: rtems_bdpart_register_from_disk sc RTEMS_SUCCESSFUL
UART:
UART: Mounting File System RTEMS_SUCCESSFUL
UART:
UART: Thread 1 created
UART:
UART: Creating file /mnt/sdcard/myfile
UART:
UART: Writing 5242880 bytes to file
UART:
UART: Perform fsync
UART:
UART: Closing file
UART:
UART: Opening file /mnt/sdcard/myfile
UART:
UART: Read 5242880 characters
UART:
UART: Verifying data...
UART:
UART: Card successfully unmounted
UART:





User interaction
==================

