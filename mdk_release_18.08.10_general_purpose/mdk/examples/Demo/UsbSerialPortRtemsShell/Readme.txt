UsbSerialPortRtemsShell

Supported Platform
===================
Myriad2 - This example works on Myriad2 silicon.

Overview
==========
CDC class example that provides access to RTEMS shell.

Hardware needed
==================
Myriad2 - This software should run on MV182 and MV212 boards.

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

User interaction
==================
    - use a tool that is capable to connect to a serial terminal

    Examples:
        Linux:
            Using socat
            - check the device detection: type "dmesg" in the command line and check
            for a device labeled MCCI and it's identifier (of type "tty...", usually "ttyACM0")
            - type the following command:
                sudo socat - /dev/ttyACM0,echo=0,raw
            - RTEMS command prompt should be printed: [/] #
            - type an RTEMS shell command
            - Example:
                [/] # help
                help
                help: ('r' repeat last cmd - 'e' edit last cmd)
                  TOPIC? The topics are
                   help, misc, mem, files, rtems, monitor
                [/] #
            Using minicom:
            - follow the same steps as above to identify the device
            - type the following command:
                sudo minicom -D /dev/ttyACM0

            Note: some linux systems may identify the device as a modem and will
                  try to communicate with it using AT commands. To avoid this an
                  udev rule should be added for the device by creating a file
                  /etc/udev/rules.d/94-serial.rules with the following content:
                  SUBSYSTEM=="usb", ATTRS{idProduct}=="f408", ATTRS{idVendor}=="040e",MODE="0666", ENV{ID_MM_DEVICE_IGNORE}="1"

        Windows: putty tool
            - in order to identify the device, Windows needs an .inf file to be provided; the
            example contains an .inf file: serial_port.inf, that can be used to load serial
            port drivers for Windows.
            The following steps should only be done once, the first time the device is used
            - to install the driver:
                - go into device manager and select Serial Port Demo device
                - choose "Update Driver Software"->"Browse my computer for driver software"->
                "Let me pick from a list of device drivers on my computer"
                - select "Ports (COM & LPT)" from the list
                - click Next and then "Have Disk"
                - browse to the location of serial_port.inf (that should be
                <mdk_installation>/examples/Demo/UsbSerialPortRtemsShell/serial_port.inf)
                - select the .inf file and click Open
                Note: starting from Windows 7 you have to disable driver signature check before
                executing the steps mentioned above.

            Once the driver is installed the application can be run.

                        - download putty from here: http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html
                        - check device manager: a modem device labeled "USB serial port (COMx)" should be detected,
            where x stands for the port number allocated for the device
                        - open putty
                        - check Serial in the Session menu
            - update "Serial line" field with the port number found in the previous step
                        - check "Implicit LF in every CR" in Terminal menu
                        - set Local echo and Local line editing to "Force on" in Terminal menu
                        - click Open to open the terminal
                        - RTEMS shell should now be opened - type any command to see that it works
            - Example:
                [/] # help
                help
                help: ('r' repeat last cmd - 'e' edit last cmd)
                  TOPIC? The topics are
                   help, misc, mem, files, rtems, monitor
                [/] #
