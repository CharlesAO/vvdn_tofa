Steps:


1.  Connect the Olimex JTAG adapter to the PC USB port and install the driver from the folder when requested by Windows

2.  WARNING: The Olimex requires TWO drivers, ftdibus.inf and ftdiport.inf to be installed, Windows will initiate the 
    installation sequence for both once the adapter is connected to the PC.

3.  For each driver follow the instructions in the Windows driver install sequence selecting the following options:

    a.  Select install from a specific location & Next button.

    b.  Select "Don't search. I will choose the driver to install." and Next.

    c.  Select "Have Disk" button and enter the folder where you extracted the Olimex driver (at step 1 above)

    d.  Select Next, The Olimex OpenOCD JTAG interface should be automatically selected; Select Next button

    e.  A security warning screen will pop-up; Select "Continue Anyway"

    f.  Select Finish, the Olimex drivers are now installed.

    g.  The Windows driver install should start again for the 2nd (serial port) driver.

4.  Power up the hardware (USB cable)

5.  Connect the Olimex to the PC USB port; connect the grey ribbon cable to the connector on Myriad daughter card. 
    The red led on the Olimex adapter should light. 


