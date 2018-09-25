MV0198PowerMeasure
=====================

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
===========
This example demonstrates how the power measure works.
It runs applications on all processors:
- LOS (RTEMS) starts LRT and wait for board initialize.
- LRT perform next operations:
	- *wait 100 ms													| no SHAVEs is running
	- starts 1 SHAVEs and wait for shave to finish 					| 1 shave is running
	- *wait 100 ms 													| no SHAVEs is running
	- starts next 3 SHAVEs and wait for shves to finish				| 3 SHAVEs are running
	- *wait 100 ms													| no SHAVEs is running
	- starts next 6 SHAVEs and wait	for SHAVEs to finish			| 6 SHAVEs are running
	- *wait 100 ms													| no SHAVEs is running

	* waits (100 ms) is used only to see in TraceProfiler the values when shves doesn't run
- LOS start a thread that measure all rails


Software description
=======================
This application uses both the Leon and the Shave code.
Basically, these are the few steps made during the application:
  1. The Clock Power reset module is initialized. The memory is also
     properly set.
  2. The SHAVEs are started and stopped 5 times.
  3. All rails are polled and recorded in ~25 ms interval.
     All shave events (e.g. RUN, RESET) are also recorded.


Hardware needed
==================
Myriad2 -This software should run on any myriad2 chip.

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
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The trace files are saved into output/profile and are read in Eclipse by log events graphics.

Visualize trace profiling results
==========================
In order to see the results, one must:
1. Open Eclipse
2. Follow the steps provided in Help -> Cheat sheets -> Myriad development -> Use Trace Profiler on a Myriad Project
3. Double click on the profile folder inside the newly created Traces folder. This should refresh the Views..
4. In order to visualize occurences of power sampling, for example, look into the Myriad Events view.

For further details regarding the TraceProfiler Eclipse usage please see moviEclipse.pdf in the moviTools documentation.

Expected output
==================
VDDCV(mA) is most relevant rail and should swing between next values:
(tested on mv0182, revision R3)
	- ~685 mA (when 6 SHAVEs are running)
	- ~500 mA (when 3 SHAVEs are running)
	- ~390 mA (when 1 shave  are running)
	- ~345 mA (when no SHAVEs is running)

VDDCV(V) should be ~0.9 V
MIPI_VDD_V (V) should be ~1.8 V


Prints
PIPE:LOS:
PIPE:LOS: ---- Measure Test App ----
PIPE:LOS: Revision detected: R4
PIPE:LOS: Measurement was finished
LOS: Application terminated successfully.
DEBUG: dumpMemoryToFile()     : (80002850 l:000E1000)[output/output_image_1280x720_u8.raw]

PIPE:LRT: Finish to download image
LRT: Application terminated successfully.

Saving trace buffers ..
