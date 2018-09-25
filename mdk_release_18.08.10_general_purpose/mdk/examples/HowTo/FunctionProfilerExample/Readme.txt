Function Profiler Example
=========================

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon.

Overview
==========
This example shows how to use function profiling.

Software description
=======================
This application uses LeonOS, LeonRT and 3 shaves.
LeonOS hosts the RTEMS. One thread from LeonOS starts the LeonRT
which in turn starts the shaves, waiting for them to finish.

Hardware needed
==================
Myriad2 -This software should run on any myriad2 chip.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version} PROFILE_INFO=function"

Where {Myriad_version} may be ma2150 or ma2450.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version} PROFILE_INFO=function"

Where {Myriad_version} may be ma2150 or ma2450.

Expected output
==================
The application output should be similar to:

PIPE:LOS: RTEMS POSIX Started
PIPE:LOS: Thread 1 created
PIPE:LOS: Thread 2 created
PIPE:LOS: Thread 3 created
PIPE:LOS: LeonRT Start Addr: 0x701C1000
PIPE:LOS: thread D start
PIPE:LOS: thread E start
PIPE:LOS: thread E ending
PIPE:LOS: LeonRT Started.
PIPE:LOS: LeonRT Stop.
PIPE:LOS: thread D ending
PIPE:LRT: All SHAVEs finished work
exit
moviDebug2 exiting
Collecting buffers ...
Saving LOS buffer profileBuffer to profileBufferLOS.bin ...
[...]
number of records: 37396
[...]
number of records: 15338
[...]
number of records: 3
[...]
number of records: 21
[...]
number of records: 17

Also, in the output/profile folder a number of generated csv files
and a lttng folder are found.
The csv files contain the profiled functions statistics, please
consult the profile component README.md for more details.
The lttng folder content consists of logs which can be viewed from Eclipse
(with trace compass plugin)

