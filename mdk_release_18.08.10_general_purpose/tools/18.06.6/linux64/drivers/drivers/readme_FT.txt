Instructions for installing the d2xx shared lib
As Linux distributions vary these instructions are a guide to installation and use.
This setup works with Mandrake 9.2 but may require some investigation on other distributions.

This library has been tested using kernel 2.4.25. 

D2XX documentation is available for the Windows dll - some variations may occur between linux and 
windows implementation. We have endevoured to make the APIs appear the same on both platforms however some
issues may slip and we would appreciate that you contact support if you observe this.

D2XX for linux was primarily developed to aid porting windows applications written with D2XX to linux.
Unfortunately the source code for D2XX is not freely available - however if you prefer to have the 
source and are starting a project from scratch you can try libftdi from Thomas Jarosch. 
Details of this library are on the ftdi web site. 


libftd2xx uses an unmodified version of libusb (http://libusb.wiki.sourceforge.net/).  Source code for libusb is included in the driver distribution in the libusb-0.1.12 directory.


Installation:
1. unzip and untar the file given to a suitable directory
gunzip libftd2xx0.4.12.tar.gz
tar -xvf libftd2xx0.4.12.tar

2. As root user copy the following files to /usr/local/lib
cp libftd2xx.so.0.4.12 /usr/local/lib

3. Change directory to /usr/local/lib
cd /usr/local/lib

4. make symbolic links to these files using the following commands:
ln -s libftd2xx.so.0.4.12 libftd2xx.so.0

5. Change directory to /usr/lib
cd /usr/lib

6. make symbolic links to these files using the following commands:
ln -s /usr/local/lib/libftd2xx.so.0.4.12 libftd2xx.so0

7. Add the following line to /etc/fstab:
none /proc/bus/usb usbdevfs defaults,devmode=0666 0 0
There have been reports that you may need to use the following command for some distros
none /proc/bus/usb usbdevfs defaults,mode=0666 0 0
none /proc/bus/usb  defaults,mode=0666 0 0 (use in 2.6 kernels)

8. Remount all in the fstab file
mount -a

If you have problems with this check with usbview (search on the internet for application 
or it can be sent to you by ftdi) to check the usb file system is mounted properly.

Other problems will be related to the ftdi_sio driver loading - 
1.you must unload this driver (and usbserial) if it is attached to your device ("rmmod ftdi_sio" and "rmmod usbserial"as root user). 
2.Your PID/VID has not been included in the distribution.A PID of 0x6006 and VID of
0x0403 should work as a temporary workaround.

-- Setting USB device permissions --

Depending on the Linux distribution and how recent it is, the method for
defining the permissions of the USB device when it's loaded by the kernel can
vary.

* Recent udev systems (most 2.6 kernel systems)

There can be variations in the way udev organises its files for setting up
rules and permissions of devices attached to the system, but the following
should cover most systems.

Do a listing of the udev rules directory

	user@host:~$ ls /etc/udev/rules.d/

There should be a file somewhere, with the word "permissions" in the name. On
some systems it could be called "40-permissions.rules", on others possibly
"020_permissions.rules". Locate the permissions file in /etc/udev/rules.d/
and, as the super user (root), open the file to edit. The author prefers nano.

      user@host:/etc/udev/rules.d$ sudo nano 40-permissions.rules
      [sudo] password for user:

Of course, editing as super user (sudo'ing) will require the ability to sudo.

The following can differ from system to system. In this case, a recent version
of Ubuntu, the file "40-permissions.rules" was present and will be edited.

In this particular permissions file there are different sections, some with
labels.

Search for the lines with LABEL="usb_serial_start" and
LABEL="usb_serial_end". In BETWEEN these two LABEL lines, insert two new lines
containing the following:

  # Permissions for ORSoC USB debugger FT2232 device
  ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="0666", GROUP="tty"

If these LABEL="" lines cannot be found (for instance, different distros have
their udev setup differently) insert the above line anywhere in the file. This
is not a definite way of enabling these permissions, and udev exists in many
forms and configurations, so if this does not work, please contact the author
regarding the issue.

* Fedora 9

As an example of a slightly different system, Fedora 9 does not have a
xx-permissions.rules file. The solution is to create a new file, but in this
case choose the name "51-permissions.rules". It can have just the rule listed
above:

  # Permissions for ORSoC USB debugger FT2232 device
  ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="0666", GROUP="tty"

The reason for the filename change is that the udev rules files are read in
lexical order (i.e. the order they would appear in a dictionary). With Fedora
9 the old FTDI driver is included as standard, and there is default rule in
50-udev-default.rules, which would override anything in 40-permissions.rules
(since it would be read first). By using the name 51-permissions.rules, our
rule will override anything set in 50-udev-default.rules.

If you find problems with permissions, then check that no later rules files
are overriding settings.

The devices whose permissions are set are /dev/ttyUSB0 /dev/ttyUSB1, the
/dev/usbdevnnn for the USB slot (nnn) being used, and the files in
/dev/bus/usb/nnn/xxx. They should all be in group tty and have permission
rw-rw-rw.

* Older systems, (early udev, devfs)
  
None of these systems were tested, however it should be easy enough to locate
the device by the vendor ID and product ID on the USB bus. See the Udev
instructions above for these values.

* Reloading the ftdi_sio driver

If it is desired that the D2XX drivers be unloaded and the original ftdi_sio one
reactivated (recreating the two /dev/ttyUSB devices), it is as simple as
removing and replacing the USB dongle from the system. However, it can also be
done at the prompt by first running as root "modprobe -r ftdi_sio", and then
"modprobe ftdi_sio vendor=0x0403 product=0x6010", which totally removes and
then reloads the device.

* Platforms tested, and known to be working, on:
  Ubuntu 8.04
  Debian 

Known issues:

2.6 kernels require a usb reset to be performed prior to opening a device 
otherewise subsequent opens will not be successfull.
This can cause a break (if there is a mixture of opened and unopened devices in 
the system) in communication with any other opened device.
FT_Listdevices and FT_CreateDeviceInfoList will also have this effect.
