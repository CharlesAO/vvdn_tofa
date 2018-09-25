EFuse Programmer example

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The EFuse high-level API is used to program the EFuses to replicate MA2x5x's
GPIO Boot mode 0x2 with additional configuration for MA2155-specific
Secure Boot.
Dummy AES and ECC keys are used for demonstration purposes. Because programming
the EFuses with these values will result in an unusable ma2155, the application
is disabled by default.
It is recommended that the application be modified with the desired EFuse
configuration, and the functionality enabled by commenting out the definition of
the EXECUTION_GUARD macro before re-running.
The application uses a GPIO pin to control power to Vddq. Either the
EFUSE_VDDQ_GPIO_EN macro must be changed to the relevant GPIO number, or
efuseVddqSetState() function must be modified if a different control mechanism
is to be used.

This example accompanies the 'EFuse Programming' application note.

Software description
=======================
An example of using the EFuse high-level API to program EFuse values.

Hardware needed
==================
Myriad2 - This software requires a mechanism to control power to the Vddq pin.
It will not function correctly on a MV0182 board as it does not have this support.

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
The results consist of printf messages seen using the debugger.


The message displayed should be:
UART: EFuses successfully programmed and verified successfully
UART: EFuses successfully locked

If an error occurs, or if the programming functionality has not been enabled as
per the instructions above, then the output will differ.
