UsbLoaderApp - A simple flash write-read application

Hardware needed
===================
MV0182 or MV0212 board, Windows PC

Overview
==========
A simple boot loader.

Software description
=======================
The application loads a prepared elf file over USB to the board and executes it.
The application will check for a global flag in flash (at address 0x7F000).
If the flag is set to 1, there is already an image in the flash and we execute it.
If the flag is 0, we wait for new image over USB.
Once a valid application is loaded in flash, it will be executed each time.

Build
==================
Please type "make help" if you want to learn available targets.

To build the project please type:
"make clean"
"make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Myriad2 - To run the application:
    - Set the proper boot mode on the MV0182 or MV0212 board, using DIPSW S1 Boot CTRL/Boot Config set it to:
        DIP - S1
        1 2 3 4 5 6 7 8
        0 0 0 0 1 1 0 0
        0 = off
        1 = on
    - open a terminal and type "make start_server"
    - OPTIONAL: open another terminal and type "make run MV_SOC_REV={Myriad_version}" to test if the application starts. The application should print some messages(see bellow) if the it runs correctly. 
    - The application can be flashed on the board so it can be used without the debugger. In order to flash the application into the board's EEPROM,first clear the EEPROM by typing "make flash_erase MV_SOC_REV={Myriad_version}", then type "make flash  MV_SOC_REV={Myriad_version}".
    - After the flashing is finished, disconnect the debugger, power cycle the board and connect it to a PC. (The flashing step is required only once).

Host PC:
    - A Windows machine is required to load the application.
    - If this is the first time, install the evaldfu driver, this can be found in mdk inside packages/movidius/usb/bin/drivers/evaldfu-V5_38_8_0-customer-free-expires-yyyy-mm-dd.zip.
    - After installing the driver "MCCI(r) USB Generic Drivers (edfu) (DFU Support)", check device detection on the Device Manager -> Universal Serial Bus
    controllers as “DFU firmware in download mode”
    - Start dfuapp.exe(dfuapp64.exe for 64-bit Windows systems). These applications can be found in mdk inside packages/movidius/usb/bin/apps/dfuapp-V1_16.zip
    - Select the DFU device from the drop-down menu (DUMMY).
    - It is recommended to strip the applications from debug information to save reduce their size. Please use "strip -g" command.
    - The files, that are going to be uploaded to the Myriad board need some information appended to them. This is done by the dfuapp application and
    is required only once per file. Please use the button "Add file suffix" and select the file from the file selection dialog box.
    This operation will append some data needed for transmitting over the USB.
    - Use the "Goto download mode" button and select the file you want to download.
    - The download should finish with the message "Download is successful". Please unplug the USB cable after receiving the message.
    (currently there is a bug where booting the new application makes Windows machines blue-screen, unplugging the USB cable avoids this)
    - After the download was finished, the app will be written to EEPROM and executed.
    - Please note that the firmware update could take more than 1 minute for a 1MB application.

Expected output
==================
The Myriad board should be detected by the PC and appear as a “DFU firmware in download mode” in the Device Manager.
Downloading new firmware should be successful.

If the application is started using make run, the following messages should be shown:
UART: 
UART: RTEMS OS Started
UART: Opening /dev/spi.flash ...
UART: 
UART: Flash App...
UART: 
UART: App ID: 0
UART: Reading Header: 24 Bytes from 0007F000
UART: No valid application in memory !!!
UART: Close and exit!
UART: No Application in flash! Waiting for update...


