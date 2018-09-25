SampleProfilerExample
=====================

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon and
ma2150, ma2450 simulator

Overview
===========
This example shows how the sampleProfier should be used.
This is also used for testing the sampleProfiler: Verifies if function durations are in the error interval.
LeonOS will start SHAVEs 0 - 7. After the execution of SHAVEs started from LeonOS is finish, LeonRT will start
and wait for SHAVEs 4-11. SHAVEs 4-7 are used by both Leons. Because of this, the results for those shaves
will contain the spend time for functions twice: one for the case when shave was started from LRT - the function
 name will prefixed with "lrt", and one for the case when SHAVEs was started from LOS - without prefix.

Software description
=======================
This application uses LeonOS, LeonRT and all the shaves.
Basically, these are the few steps made during the application:
  1. The Clock Power reset module is initialized. The memory is also
     properly set.
  2. The SHAVEs are started to execute the function "profile", using the swcStartShave function.
  3. The profiler overrides the swcStartShave and swcShaveStartAsync (this function is used by OsDrvSvuRunShave)
  functions and will start the profiling of the started shave.If rtems is defined, the overwriten functions
  will use rtems drivers, otherwise will use bare metal drivers.
  4. The profiler also overrides the swcWaitShave function (only for LeonRT) and will stop profiling of
  the shave when the shave finished processing.

Hardware needed
==================
Myriad2 - This software should run on any myriad2 chip.

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

NOTE: Due to the larger code size, this app cannot build in debug mode, therefore an additional flag has been added to ingore the MV_BUILD_CONFIG=debug parameter.

Setup
==================
Please read the README.md placed in mdk/common/components/profiler before using this example.

Myriad2 simulator - To run the application:
    - open terminal and type "make start_simulator MV_SOC_REV={Myriad_version}"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version} PROFILE_INFO=sample"
Myriad2 silicon - To run the application:
    - open terminal and type make "start_server"
    - open another terminal and type "make run MV_SOC_REV={Myriad_version} PROFILE_INFO=sample"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.
If you need to profile a debug session, in terminal type make debug MV_SOC_REV={Myriad_version} PROFILE_INFO=sample

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

How To Use
============
  1. Depending on the total duration of the shave functions, the SAMPLE_DURATION_MS variable
     can be overridden. Please see the Makefile.
  2. The SAMPLE_PERIOD_MICRO can also be overriden in the Makefile to use smaller DDR space.
     Please read the Readme in tha functionProfiler component to see more details


Expected output
==================
You should see the following output in the debugger:

UART: Starting profiler example...
UART: Starting shave 0 from LOS rtems task...
UART: Hello from Shave
UART: Shave 0 execution finished in LOS rtems task.
UART: Starting shave 1 from LOS rtems task...
UART: Hello from Shave
UART: Shave 1 execution finished in LOS rtems task.
UART: Starting shave 2 from LOS rtems task...
UART: Hello from Shave
UART: Shave 2 execution finished in LOS rtems task.
UART: Starting shave 3 from LOS rtems task...
UART: Hello from Shave
UART: Shave 3 execution finished in LOS rtems task.
UART: Starting shave 4 from LOS rtems task...
UART: Hello from Shave
UART: Shave 4 execution finished in LOS rtems task.
UART: Starting shave 5 from LOS rtems task...
UART: Hello from Shave
UART: Shave 5 execution finished in LOS rtems task.
UART: Starting shave 6 from LOS rtems task...
UART: Hello from Shave
UART: Shave 6 execution finished in LOS rtems task.
UART: Starting shave 7 from LOS rtems task...
UART: Hello from Shave
UART: Shave 7 execution finished in LOS rtems task.
UART: Starting LRT..
UART: LRT started..
UART: Starting Shave 4..
UART: Hello from Shave
UART: Shave 4 finished.
UART: Starting Shave 5..
UART: Hello from Shave
UART: Shave 5 finished.
UART: Starting Shave 6..
UART: Hello from Shave
UART: Shave 6 finished.
UART: Starting Shave 7..
UART: Hello from Shave
UART: Shave 7 finished.
UART: Starting Shave 8..
UART: Hello from Shave
UART: Shave 8 finished.
UART: Starting Shave 9..
UART: Hello from Shave
UART: Shave 9 finished.
UART: Starting Shave 10..
UART: Hello from Shave
UART: Shave 10 finished.
UART: Starting Shave 11..
UART: Hello from Shave
UART: Shave 11 finished.
UART: LRT stoped. LOS exiting...
UART: Sample profiler example executed successfully
LOS: LeonOS (P0:ALOS) suspended at 0x70191104 (Application terminated successfully)

After the application terminated the moviProf prints the functions and their respective duration in ms.

Also, in the output folder you will see .gprof files.

