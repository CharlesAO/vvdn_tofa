AsyncPipe_x2_concurrent

Overview
====================
This app illustrates concurrent operation of multiple pipelines on SIPP HW using the Non-blocking
SIPP framework APIs.

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
The output in this case is a console listing of the frames of each pipe as they progress.
Pipeline 1 has been deliberately chosen to be significantly less complex than pipeline 0 so that it may be observed
that while pipeline 0 is started first, pipeline 1 will overtake it and complete several frames while pipeline 0 is
operating on its first frame to demonstrate concurrent operation.

Note - a build define is provided - CONCURRENCY_TEST_CREATE_CONFLICT in main.c. This may be set non-zero to create
a resourcing conflict between the pipelines.
If this is enabled then the SIPP framework's scheduler will find itself unable to schedule the pipelines concurrently
and instead they will fall back to a serial mode of operation which the console trace will confirm.

Expected displayed message at the end of run: "moviUnitTest:PASSED"
