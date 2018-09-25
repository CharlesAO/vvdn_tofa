sippPerfTestBed

Overview
====================
This app provides a farmework in which to measure et he perfomance of sipp ipelines and acts
as a demonstartion of the runtime profiling enabled within the framework. Using the data gathered
from this exercise the framework's analysis algorithm may be called to identify the bottleneck
within the pipeline

4 configurations for the test are provided, A->D. These configurations may be worked through in
conjunction with reading the section on SIPP perfomance measurement and analysis within the
MDK-ma2x5x-SippUsersGuide.

Hardware needed
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Build
====================
Change directory to the required build platform.

build/myriad2 (mv0212 or mv0182 board): To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
====================
MV0182
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"


Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Notes
====================
This is a LeonRT demonstration example. It can not be run from within Eclipse and will not work if flashed
into EEPROM unless example is changed to run on LeonOS and interrupts are re-routed to LeonOS.

Expected output
====================

For each pipeline in the test the framework will analyse the results and porvde feedback as to whether it believed HW kernels, SW kernels, the CMX DMA or the framework itself were the bottleneck in the system.
This will be conveyed through a trace message such as...

"** HW most often long-pole **"

By setting SIPP_RT_STATS_PRINT_CTRL in the appropriate cfg header file additional trace for each iteration
of the schedule will be produced. Further a log file named perfTestBedLog.txt is produced to detail the
timings of the start and completion of each frame for each pipeline in the test. This can illustrate the crossover between concurrent streams.
