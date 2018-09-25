XLink threadded application

Supported Platform
==================
The platform is dependent on the MDK release package installed.
Myriad2 examples will not build/run on MyriadX releases and vice-versa.

Myriad2 - This example works on Myriad2: ma2150, ma2450
MyriadX - This example works on MyriadX: ma2480

Overview
==========
Sends random messages from PC to the Myriad. The Myriad receives messages from PC and sends them back.
When the PC gets the messages back, it compares them to the original sent buffers. Prints 'TEST PASSED' or 'TEST FAILED' based on the result.

Software description
=======================
This test comprises of a PC application and a Myriad application
1) PC Application
This includes code for the pc application that does the following:
    1. Boots the Myriad
    2. Initializes XLink (including sending a ping from the PC)
    3. Opens a named stream
    4. Starts a thread for receiving back messages. This does the following:
        a) Reads the message from the USB
        b) Checks the first character of the message to know what to compare it to
        c) Compare the recd message to the data in the correct part of the buffer
        d) Store the pass/fail value
        e) Release the message received
    5. Starts NUM_THREADS threads for sending messages. This does the following NUM_MESSAGES times:
        a) Get the part of the buffer to send
        b) Populates the first character of the buffer with the message number
        c) sends the message
    6. Resets PC application and the remote (myriad)
2) Myriad Application
This includes leon code for LeonOS. The steps in this application are:
    1. The Clock Power reset module is initialized
    2. Initializes XLink (including sending a ping from the Myriad)
    3. Opens a named stream
    4. A thread that receives and returns NUM_MESSAGES messages is started:

Hardware needed
==================
Depending on MDK release installed:
- Myriad2 (ma2150 or ma2450)
- MyriadX (ma2480) with USB connectivity.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 / MyriadX - To build the project, please cd to myriad/ and type:
        - "make clean"
        - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150, ma2450 or ma2480.
The default Myriad revision in this project is ma2450

PC      - To build the PC side, please cd to pc/ and type:
        - make

Setup and Run
==================
1) Ensure your udev rules are set correctly. Add the following lines to /etc/udev/rules.d/98-mcci.rules:
◦ SUBSYSTEM=="usb", ATTRS{idProduct}=="2150", ATTRS{idVendor}=="03e7", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
◦ SUBSYSTEM=="usb", ATTRS{idProduct}=="2485", ATTRS{idVendor}=="03e7", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
◦ SUBSYSTEM=="usb", ATTRS{idProduct}=="f63b", ATTRS{idVendor}=="03e7", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
◦ SUBSYSTEM=="usb", ATTRS{idProduct}=="f63b", ATTRS{idVendor}=="040e", MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"
If 98-mcci.rules file doesn't exist, please create it.
Run: sudo udevadm control --reload-rules && sudo udevadm trigger
See MDK document XLink_UserGuide.pdf for more details.

2) Configure your board boot switches(USB Bootmode)
    Myriad2 (2, 7 up):
    | |-| | | | |-| |
    |-| |-|-|-|-| |-|
     1 2 3 4 5 6 7 8
    MyriadX (2, 4, 5 up):
    | |-| |-|-| | | |
    |-| |-| | |-|-|-|
     1 2 3 4 5 6 7 8

3) To run the application:
    - cd to pc/ and type './XLink'

Expected output
==================
Device Found name 4-ma2450 
Myriad was booted
Initialize done
XLinkConnect done - link Id 0
Reading back data... Sending some data...
SENDING3
Data sent
Data read
READING msg 3
Reading back data... Sending some data...
Sending some data...
SENDING4
Data sent
SENDING0
Data sent
Data read
READING msg 4
Reading back data... Data read
READING msg 0
Reading back data... Sending some data...
SENDING9

(...)

Reading back data... Sending some data...
SENDING97
Data sent
Data read
READING msg 97
Reading back data... Sending some data...
SENDING90
Data sent
Data read
READING msg 90
Reading back data... Sending some data...
SENDING98
Data sent
Data read
READING msg 98
Reading back data... Sending some data...
SENDING96
Data sent
Data read
READING msg 96
TEST PASSED! :)
