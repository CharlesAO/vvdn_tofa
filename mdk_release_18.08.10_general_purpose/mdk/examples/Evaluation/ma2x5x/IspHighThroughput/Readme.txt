Opipe ISP throughput & power evaluation

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This example shows the OPIPE HW power consumption and throughput of a full ISP pipeline.

Software description
=======================
A raw 13MP frame is loaded in DDR as input image.
An ISP pipeline that takes the input raw image and debayers it.
The input and output images' buffers are located in DDR for this example.
For the consistency of the measurements, the same image is processed ten times.
At each iteration, the frame rate, temperature and power consumption are measured.
Also, CRC is made for the output frame and is compared with the expected value.
An output YUV420 8bit image is also saved.

Hardware needed
==================
Myriad2 -This software should run on MV182, MV212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

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
    - open another terminal and type "make run MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

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

Expected output
==================
The results consist in printf seen using the debugger:
For each iteration, the following information is printed:
- frame rate, in frames/second
- number of clock cycles/frame
- number of clock cycles/pixel
- troughput, in megapixels/second
- core power consumption
- DDR power consumption
- chip temperature
At the end, a CRC check is done to validate the result.
Also, the generated YUV image can be visually inspected for correctness.

Example:

Ma2150
====================
DEBUG: unitTestVerbosity()    : VERBOSITY_QUIET

DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: Read test image (10bit bayer)
DEBUG: loadMemFromFile()      : o:000000 l:26257920 -> 0x812C7F80 [../../../../common/../resources/Studio_D65_800_IMX214_2_002.raw]
UART:
UART:  Running at 600MHz, Image Resolution = 4208x3120
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.188 fps, 14929701 cc/frame, 1.137 cc/pixel, 527.631 MP/s
UART: Core power = 916.32mW, DDR power = 120.87mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.192 fps, 14928314 cc/frame, 1.137 cc/pixel, 527.680 MP/s
UART: Core power = 916.23mW, DDR power = 120.81mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.194 fps, 14927425 cc/frame, 1.137 cc/pixel, 527.712 MP/s
UART: Core power = 917.66mW, DDR power = 120.72mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.205 fps, 14923576 cc/frame, 1.137 cc/pixel, 527.848 MP/s
UART: Core power = 916.23mW, DDR power = 120.72mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.188 fps, 14929759 cc/frame, 1.137 cc/pixel, 527.629 MP/s
UART: Core power = 916.32mW, DDR power = 120.72mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.195 fps, 14927330 cc/frame, 1.137 cc/pixel, 527.715 MP/s
UART: Core power = 916.23mW, DDR power = 120.81mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.190 fps, 14929071 cc/frame, 1.137 cc/pixel, 527.653 MP/s
UART: Core power = 916.23mW, DDR power = 120.81mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.180 fps, 14932974 cc/frame, 1.137 cc/pixel, 527.516 MP/s
UART: Core power = 916.23mW, DDR power = 120.81mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.197 fps, 14926650 cc/frame, 1.137 cc/pixel, 527.739 MP/s
UART: Core power = 916.32mW, DDR power = 120.66mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
UART: 40.188 fps, 14929877 cc/frame, 1.137 cc/pixel, 527.625 MP/s
UART: Core power = 916.32mW, DDR power = 120.66mW, On-die junction temperature: CSS=34.5C, MSS=32.8C, UPA0=33.9C, UPA1=33.9C
UART:
DEBUG: unitTestAssert()       : (value:0x00000001)                            => PASS
DEBUG:
DEBUG: moviUnitTest:PASSED

DEBUG:
UART: Saving output image...
DEBUG: dumpMemoryToFile()     : (80000000 l:012C7F80)[Lab_8bpp_4208x3120_P420.yuv]

LRT: LeonRT (P0:ALRT) suspended at 0x70182158 (Application terminated successfully)
UART: Memory dumping finished.


Ma2450
====================

PIPE:LOS: IspHighThroughput benchmark
PIPE:LOS: ===============================================================
PIPE:LOS: Resolution: 4208x3120
PIPE:LOS: Frequency:  600MHz
PIPE:LOS:
PIPE:LOS: Loading 13MP file...        (estimated loading time: 2min)
DEBUG: loadMemFromFile()      : o:000000 l:26257920 -> 0x812C7F80
[../../../../common/../resources/Studio_D65_800_IMX214_2_002.raw]
PIPE:LOS:
PIPE:LOS: Processing 10 iterations
PIPE:LOS:
PIPE:LOS: 40.229 fps, 14914737 cc/frame, 1.136 cc/pixel, 528.161 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.57mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.238 fps, 14911096 cc/frame, 1.136 cc/pixel, 528.290 MP/s
PIPE:LOS: Core power = 928.05mW, DDR power = 207.18mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.226 fps, 14915725 cc/frame, 1.136 cc/pixel, 528.126 MP/s
PIPE:LOS: Core power = 928.05mW, DDR power = 207.30mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.243 fps, 14909320 cc/frame, 1.136 cc/pixel, 528.352 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.39mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.229 fps, 14914696 cc/frame, 1.136 cc/pixel, 528.162 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.30mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.254 fps, 14905429 cc/frame, 1.135 cc/pixel, 528.490 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.27mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.234 fps, 14912943 cc/frame, 1.136 cc/pixel, 528.224 MP/s
PIPE:LOS: Core power = 926.37mW, DDR power = 207.30mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.248 fps, 14907576 cc/frame, 1.135 cc/pixel, 528.414 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.36mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.201 fps, 14924848 cc/frame, 1.137 cc/pixel, 527.803 MP/s
PIPE:LOS: Core power = 927.87mW, DDR power = 207.21mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
PIPE:LOS:
PIPE:LOS: 40.213 fps, 14920402 cc/frame, 1.136 cc/pixel, 527.960 MP/s
PIPE:LOS: Core power = 928.05mW, DDR power = 207.30mW, On-die junction temperature: CSS=33.9C, MSS=32.6C, UPA0=32.1C, UPA1=33.4C
DEBUG: unitTestAssert()       : (value:0x00000001)
=> PASS
PIPE:LOS:
PIPE:LOS: ===============================================================
PIPE:LOS: Validating test...
PIPE:LOS: Saving output image - will take about 3 minutes ...
DEBUG: dumpMemoryToFile()     : (80000000
l:012C7F80)[Lab_8bpp_4208x3120_P420.yuv]
DEBUG:
DEBUG: moviUnitTest:PASSED

DEBUG:
LRT: LeonRT (P0:ALRT) suspended at 0x7018132C (Application terminated successfully)
PIPE:LOS: Memory dumping finished.

Note
==================
The example contains specific power reduction techniques which makes the example very sensitive,
DEBUG: so small changes could lead to reduction of the pipe performance.
