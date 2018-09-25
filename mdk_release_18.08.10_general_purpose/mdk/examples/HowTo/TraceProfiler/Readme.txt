TraceProfiler

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and ma2150, ma2450 simulator

Overview
==========
This example demonstrates how the trace profiler works.
It runs applications on all processors:
- LOS (RTEMS) starts LRT and then starts 6 SHAVEs (6-11) which run convolution on the input image.
- LRT also starts 6 other SHAVEs (0-5) in parallel with LOS which are also processing the input image.

Software description
=======================
This application uses both the Leon and the Shave code.
Basically, these are the steps made during the application:
    1. Configure the system by using the rtmes configure clocks.
    2. Start the LRT from LOS. The Leon RT starts the first 6 SHAVES(0-5)
    3. In the same time the LOS starts the next 6 SHAVES(6-11)

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.

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
Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The trace files are saved into output/profile and are read in Eclipse by log events graphics.

The output_1280x720_P420.yuv file will be created in the main directory where the project is run
Input and output images can be verified with the debugger, only when running with "make debug MV_SOC_REV={Myriad_version}"
Save the output by typing:
mdbg::savefile TraceProfiler.yuv out_image_los 1382400

Visualize trace profiling results
==========================
In order to see the results, one must:
1. Open Eclipse
2. Follow the steps provided in Help -> Cheat sheets -> Myriad development -> Use Trace Profiler on a Myriad Project
3. Double click on the profile folder inside the newly created Traces folder. This should refresh the Views..
4. In order to visualize occurences of power sampling, for example, look into the Myriad Events view.

For further details regarding the TraceProfiler Eclipse usage please see moviEclipse.pdf in the moviTools documentation.
