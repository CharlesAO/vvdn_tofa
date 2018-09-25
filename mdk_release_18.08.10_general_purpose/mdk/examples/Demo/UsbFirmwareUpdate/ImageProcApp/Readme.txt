ImageProcApp demo application

Overview
==========
A Demo application which supports firmware update over USB. This application could be an image processing application (that's what the name suggests), but in this case is a simple LED blinking application, demonstrating functionality whit no extra equipment necessary.

Software description
=======================
This application shows what functionality is needed in a user application for firmware update over USB.
The application support a VSC class and has a task that blinks one led. The blinking led is just for demonstration
and can be changed with any other user code, such as an image processing algorithm.
The application waits for a firmware update command over USB. When the command is received, it sets the global flag
in flash to 0 and then gives control over to the USB flasher application, which handles the actual firmware update.
Please note that the application should respect the reserved portion of DDR, which is used exclusively by the bootloader.

Hardware needed
===================
MV0182 or MV212 board, Windows PC

Build
==================
Please type "make help" if you want to learn available targets.

To build the project please type:
    "make clean"
    "make all MV_SOC_REV={Myriad_version}"

To strip the image: "make stripimg MV_SOC_REV={Myriad_version}" - this can be done only after the build of the project and is defined only on this project.

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

NOTE: To reduce the code size, this app needs to be built with size optimization, and cannot build in debug mode, therefore an additional flag has been added in the Makefile to ignore the MV_BUILD_CONFIG=debug parameter and instead use -Os as the compile optimization settings. If this optimization is not suitable then additional measures need to be taken by the developer.

Setup
==================
Myriad2 - To run the application:
    - The board should be already flashed with the "UsbLoaderApp" which is the actual bootloader;
    - Upload this application to the MV0182 or MV0212 board using dfuapp.exe (check UsbLoaderApp readme.txt for more details)
    - Connect an USB cable from the MV0182 or MV0212 board to the a PC;

Host PC:
    - A Windows machine is required for firmware upgrading;
    - If this is the first time, install the mccidemo driver.
      The driver current version can be found in mdk inside packages/movidius/usb/bin/drivers/mccidemo-V5_38_8_0-customer-free-expires-yyyy-mm-dd.zip
      If during the installation Windows complains about the driver compatibility, select "Continue anyway" and proceed with the setup.
      If the USB device is still not recognizes in the Device Manager, the driver might need to be installed manually.
      Please follow the steps below:
          - In Device Manager select the device and "Update Driver Software";
          - Select "Browse my computer for driver software";
          - Select "Let me pick from a list of device drivers on my computer";
          - Scroll to "Universal Serial Bus controllers";
          - Use the "Have Disk" button to browse to the driver;
    - Check device detection on the Device Manager -> Universal Serial Bus controllers as “MCCI(r) USB Demo Driver”;
    
User interaction
==================
    - Open the dfuapp.exe and upload the new firmware. Please check flash_test_app for more details.
    - It is recommended to strip the application from debug information to further reduce the elf file's size. To strip the image please use: "make stripimg MV_SOC_REV={Myriad_version}"

Expected output
==================
    After sending the firmware update command, the control is given to the bootloader.
    A “DFU firmware in download mode” should appear in the Device Manager -> Universal Serial Bus controllers.
    After the new firmware is successfully uploaded, it will start executing and again.
    Please note that some time should be allowed for the flash read/write to complete.
