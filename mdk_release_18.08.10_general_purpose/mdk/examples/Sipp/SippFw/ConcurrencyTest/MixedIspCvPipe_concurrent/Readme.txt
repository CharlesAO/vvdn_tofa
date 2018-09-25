MixedIspCvPipe_concurrent

Overview
====================
This app illustrates operation of multiple pipelines on SIPP HW using the blocking and Non-blocking SIPP framework APIs.
In this case three pipelines, representing some typical pipes for CV and ISP operations are executed concurrently
Two of the pipelines are configured so that they  share a HW resource, thus the framework must run them serially but
concurrently with the third pipe


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

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter. Code section is larger on ma2450

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

It should be noted that the blocking SIPP API is only accepted by the framework when there are no pending async API transactions.
In this case the message "Pipe successfully called blocking API sippProcessFrame" will be printed.

The console trace will confirm multiple rejections of the sippProcessFrame call. This will only proceed once the async API call has completed.
In this case the message "Pipe failed in call to blocking API sippProcessFrame due to active and/or pending operations" will be printed.

Expected displayed message at the end of run: "moviUnitTest:PASSED"
