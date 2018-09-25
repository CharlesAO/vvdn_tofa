SimpleCrossCompilableCVPipe

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
==========
Loads input image in DDR and applies the DOG effect on it (DOG = difference of gaussian: 2 convolutions applied separately on the same image,then an abs diff filter that
takes as input the output of the two convolutions )


Software description
=======================

This application uses both the Leon and the Shave code. Basically, these are the few steps made during the application:
    1. Initial setup.
    2. Run the algorythm on shave.
    3. Determine the cycles number.

Basically, this application loads a data in DDR. The output image is obtained by applying the DOG algorithm  on the y plane.

When applying the DOG filters on the Y component, a simple optimization is made. A new set of data is being brought
while the previous one is send as input to the DOG filters function.



Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Go to the "myriad" subfolder.

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Go to the "myriad" subfolder

Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

The expected output on simulator and silicon:

 - the shave number of cycles is printed into the terminal
UART: SimpleCrossCompilableCVPipe algorithm.
DEBUG: loadMemFromFile()      : o:000000 l:076800 -> 0x80012C00 [../input/DunLoghaire_320x240.yuv]
UART:
UART: Shave measured instruction count: 69210309
UART:
UART: moviUnitTest:PASSED
 - Additionally, the output image can be verified with the debugger, only when running with "make debug MV_SOC_REV={Myriad_version}"

Running the app on PC
==========================
Go to the "pc/linux" or "pc/VS2010" subfolder, depending on platform to run the test on

To build the project please type:
"make clean"
"make all MV_SOC_REV={Myriad_version}"
To run the application type "make run MV_SOC_REV={Myriad_version}"

The expected output:
 - output_320x240_P400.yuv file will be created in the main directory where the project is run
Save the output by typing:
mdbg::savefile SimpleCrosscompilableCVPipe.yuv outputframe 115200

