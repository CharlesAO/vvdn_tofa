LowDynamicPower benchmark
Shaves power evaluation

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
=======================
The benchmark runs a large number of iterations on a frame, on various numbers
of SHAVEs and reports the speed and power consumed.

Software description
=======================
1. Configures the system.
2. Initialize and start the timer.
3. Run the application on the targeted Shaves.
    3.1 The SHAVEs run 3 filters on u8 input data: conv3x3, conv5x5, conv7x7.
4. Sample temperature (only for ma2150 and ma2450 silicon version), power rails and timer.
5. Check the results.. Temperature measured varies in function of chip external temperature influence.
6. Show report.

The output file can be saved with command "save out_image (1280*720) outputImage_8b.bin"
The purpose of this application is to show power measurements while running code on all SHAVEs cores.
The output image has known cosmetic artefacts (first lines of the image, and left/right margins) due to padding functionality.

Hardware needed
=======================
MV182, MV212
!!!Please make sure that you have attached the MV0198 board on the back of MV182 or MV212 board.!!!

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

* For MDK General Software Package
==================================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

* For MDK Evaluation Package
=================================
Windows:
Starting the server:
in one Command Prompt, from the root directory of the release, go to the mdk/examples/Evaluation/ma2x5x path then type startDebugServer.bat
Running the application:
in another Command Prompt, from the previously mentioned path type runApp.bat <AppName>
or
go to the application path and type run.bat

Linux:
Starting the server:
in one Terminal, from the root directory of the release, go to the mdk\examples\Evaluation\ma2x5x path then type bash startDebugServer.sh
Running the application:
in another Terminal, from the previously mentioned path type bash runApp.bat <AppName>
or
go to the application name to be run and type bash run.sh


Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
=======================
The power numbers will vary slightly.

Ma2150
=======================
UART: LowDynamicPower
UART: Benchmark scenario
UART: ============================================================
UART: Resolution: 1280x720
UART: Processing: conv7x7 + conv7x7 + conv7x7 on u8
UART: Frequency: 600Mhz
UART:
UART: Running stripe based scheduling
UART: ============================================================
UART: Starting SHAVEs (0)...done
DEBUG: loadMemFromFile()      : o:000000 l:921600 -> 0x802F7600 [../../../../common/../resources/CobKitchen_1280x720.yuv]
UART: Starting SHAVEs (1)...done
UART: Starting SHAVEs (2)...done
UART: Starting SHAVEs (4)...done
UART: Starting SHAVEs (8)...done
UART: Starting SHAVEs (12)...done
UART: Report
UART: ============================================================
UART: 1000 frames: CRC     ok,  0xSHAVEs. Result:   0 fps,  0 gflops, power  231.87 mW
UART: On-die junction temperature: CSS=39.9C, MSS=38.0C, UPA0=38.8C, UPA1=39.0C
UART: 1000 frames: CRC     ok,  1xSHAVEs. Result:  27 fps,  7 gflops, power  433.97 mW
UART: On-die junction temperature: CSS=39.9C, MSS=42.2C, UPA0=38.8C, UPA1=39.0C
UART: 1000 frames: CRC     ok,  2xSHAVEs. Result:  53 fps, 14 gflops, power  517.35 mW
UART: On-die junction temperature: CSS=39.9C, MSS=43.8C, UPA0=38.8C, UPA1=39.0C
UART: 1000 frames: CRC     ok,  4xSHAVEs. Result: 101 fps, 27 gflops, power  678.30 mW
UART: On-die junction temperature: CSS=39.9C, MSS=46.5C, UPA0=38.8C, UPA1=39.0C
UART: 1000 frames: CRC     ok,  8xSHAVEs. Result: 188 fps, 50 gflops, power 1009.11 mW
UART: On-die junction temperature: CSS=39.9C, MSS=52.6C, UPA0=38.8C, UPA1=39.0C
UART: 1000 frames: CRC     ok, 12xSHAVEs. Result: 263 fps, 71 gflops, power 1350.93 mW
UART: On-die junction temperature: CSS=39.9C, MSS=62.0C, UPA0=38.8C, UPA1=39.0C
UART:
UART: moviUnitTest:PASSED


Ma2450
=====================
PIPE:LOS: LowDynamicPower benchmark
PIPE:LOS: ============================================================
PIPE:LOS: Resolution: 1280x720
PIPE:LOS: Processing: conv7x7 + conv7x7 + conv7x7 on u8
PIPE:LOS: Frequency: 600Mhz
PIPE:LOS:
PIPE:LOS: Running stripe based scheduling
PIPE:LOS: ============================================================
DEBUG: loadMemFromFile()      : o:000000 l:921600 -> 0x802F7600 [../../../../common/../resources/CobKitchen_1280x720.yuv]
PIPE:LOS: Starting SHAVEs (0)...done
PIPE:LOS: Starting SHAVEs (1)...done
PIPE:LOS: Starting SHAVEs (2)...done
PIPE:LOS: Starting SHAVEs (4)...done
PIPE:LOS: Starting SHAVEs (8)...done
LOS: LeonOS (P0:ALOS) suspended at 0x701A032C (Application terminated successfully)
PIPE:LOS: Starting SHAVEs (12)...done
PIPE:LOS: Report
PIPE:LOS: ============================================================
PIPE:LOS: 1000 frames:           ,  0xSHAVEs. Result:   0 fps,  0 gflops, power  342.03 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: 1000 frames:           ,  1xSHAVEs. Result:  27 fps,  7 gflops, power  447.23 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: 1000 frames:           ,  2xSHAVEs. Result:  53 fps, 14 gflops, power  527.55 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: 1000 frames:           ,  4xSHAVEs. Result: 101 fps, 27 gflops, power  682.75 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: 1000 frames:           ,  8xSHAVEs. Result: 188 fps, 50 gflops, power  992.06 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: 1000 frames:           , 12xSHAVEs. Result: 263 fps, 71 gflops, power 1296.97 mW
PIPE:LOS: On-die junction temperature: CSS=32.8C, MSS=31.9C, UPA0=31.5C, UPA1=31.5C
PIPE:LOS: moviUnitTest:PASSED
