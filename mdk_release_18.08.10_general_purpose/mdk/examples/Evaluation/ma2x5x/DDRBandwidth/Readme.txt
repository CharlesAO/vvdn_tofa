Memory Performance Benchmark

Supported Platform
===================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon.

Overview
==========
This application measure the memory performance Shave using either cores or DMA.

Purpose
==========
The main purpose of this application is the show to the user some memory bandwidth figures, so they know what to expect.

Software description
=======================
This application is doing different kinds of transfers and is benchmarking them.
In preparation for the benchmarks, clocks are started, caches are partitioned and the system clock is printed.

This application is doing different kinds of transfers and is benchmarking them.
1. The first benchmark shows the memory access speed from an individual SHAVE processor via memcpy operations.
2. Second benchmark shows the memory access speed from using the CMX DMA to move some chunks of contiguous bytes in memory.
3. Third benchmark targets the system bandwidth when all SHAVEs copy data simultaneously from CMX to same slice of CMX, showing that 12 parallel transfers are almost as efficient as one transfer.

Please note that all transfers are verified afterwards, and in case some tests fail, you are notified accordingly at the end of all benchmarks.

Hardware needed
==================
Myriad2 -This software should run on MV182, MV212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

To build the project please type:
"make clean"
"make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Setup
==================

* For MDK General Software Package
==================================
Myriad2 silicon - To run the application you need two separate terminals:
    - open one terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}" or "make run MV_SOC_REV={Myriad_version}"

Explanation of commands:
"make start_server" - This will start a local server that waits for commands and forward them to the board over the debugger
"make run MV_SOC_REV={Myriad_version}"          - This will compile the application if not already done and issue a command to the command to run the elf file created in the build process
"make debug MV_SOC_REV={Myriad_version}"        - Same as "make run MV_SOC_REV={Myriad_version}" but after the application exit you will get the chance to issue commands to the debugger. Please read debugger user manual for a list of commands.

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


Output for 2150:
==================
PIPE:LOS: 
PIPE:LOS: DDR bandwidth benchmark
PIPE:LOS: 
PIPE:LOS: System running at 600 Mhz
PIPE:LOS: DDR    clocked at 528 Mhz
PIPE:LOS: 
PIPE:LOS: 1. Benchmark shave memcpy    
PIPE:LOS: Shave 0 will transfer memory via CPU
PIPE:LOS:   1KB DDR to DDR  394.5707 MB/s
PIPE:LOS:   1KB CMX to DDR  943.5387 MB/s
PIPE:LOS:   1KB DDR to CMX  669.6428 MB/s
PIPE:LOS:   1KB CMX to CMX 2615.7925 MB/s
PIPE:LOS:  32KB DDR to DDR  498.2726 MB/s
PIPE:LOS:  32KB CMX to DDR  977.8357 MB/s
PIPE:LOS:  32KB DDR to CMX  984.0970 MB/s
PIPE:LOS:  32KB CMX to CMX 3947.3684 MB/s
PIPE:LOS: 128KB DDR to DDR  503.4706 MB/s
PIPE:LOS: 128KB CMX to DDR  979.8414 MB/s
PIPE:LOS: 128KB DDR to CMX 1004.6617 MB/s
PIPE:LOS: 128KB CMX to CMX 3996.5894 MB/s
PIPE:LOS: 
PIPE:LOS: 2. Benchmark shave DMA copy  
PIPE:LOS: Shave 0 will transfer memory via DMA
PIPE:LOS:   1KB DDR to DDR 1008.4983 MB/s
PIPE:LOS:   1KB CMX to DDR 1233.5526 MB/s
PIPE:LOS:   1KB DDR to CMX  955.8524 MB/s
PIPE:LOS:   1KB CMX to CMX 1171.8750 MB/s
PIPE:LOS:  32KB DDR to DDR 1570.4832 MB/s
PIPE:LOS:  32KB CMX to DDR 3580.9775 MB/s
PIPE:LOS:  32KB DDR to CMX 3392.4370 MB/s
PIPE:LOS:  32KB CMX to CMX 4210.6445 MB/s
PIPE:LOS: 128KB DDR to DDR 1582.7458 MB/s
PIPE:LOS: 128KB CMX to DDR 3640.0701 MB/s
PIPE:LOS: 128KB DDR to CMX 3617.5959 MB/s
PIPE:LOS: 128KB CMX to CMX 4477.3447 MB/s
PIPE:LOS: 
PIPE:LOS: 3. Benchmark multiple SHAVEs DMA 
PIPE:LOS: Multiple shave processors transfer memory simultaneously via DMA
PIPE:LOS:  1 SHAVEs 16KB CMX to DDR 3098.51 MB/s
PIPE:LOS:  1 SHAVEs 16KB DDR to CMX 3523.24 MB/s
PIPE:LOS:  2 SHAVEs 16KB CMX to DDR 3702.12 MB/s
PIPE:LOS:  2 SHAVEs 16KB DDR to CMX 3656.81 MB/s
PIPE:LOS:  4 SHAVEs 16KB CMX to DDR 3697.13 MB/s
PIPE:LOS:  4 SHAVEs 16KB DDR to CMX 3654.97 MB/s
PIPE:LOS:  8 SHAVEs 16KB CMX to DDR 3691.84 MB/s
PIPE:LOS:  8 SHAVEs 16KB DDR to CMX 3658.96 MB/s
PIPE:LOS: 12 SHAVEs 16KB CMX to DDR 3693.40 MB/s
PIPE:LOS: 12 SHAVEs 16KB DDR to CMX 3658.93 MB/s
PIPE:LOS:  1 SHAVEs 32KB CMX to DDR 3370.91 MB/s
PIPE:LOS:  1 SHAVEs 32KB DDR to CMX 3579.63 MB/s
PIPE:LOS:  2 SHAVEs 32KB CMX to DDR 3462.44 MB/s
PIPE:LOS:  2 SHAVEs 32KB DDR to CMX 3654.40 MB/s
PIPE:LOS:  4 SHAVEs 32KB CMX to DDR 3638.26 MB/s
PIPE:LOS:  4 SHAVEs 32KB DDR to CMX 3650.09 MB/s
PIPE:LOS:  8 SHAVEs 32KB CMX to DDR 3683.30 MB/s
PIPE:LOS:  8 SHAVEs 32KB DDR to CMX 3652.64 MB/s
PIPE:LOS: 12 SHAVEs 32KB CMX to DDR 3698.02 MB/s
PIPE:LOS: 12 SHAVEs 32KB DDR to CMX 3654.74 MB/s
PIPE:LOS: All transactions were verified and passed
PIPE:LOS: All benchmarks finished


Output for 2450:
==================
PIPE:LOS: 
PIPE:LOS: DDR bandwidth benchmark
PIPE:LOS: 
PIPE:LOS: System running at 600 Mhz
PIPE:LOS: DDR    clocked at 732 Mhz
PIPE:LOS: 
PIPE:LOS: 1. Benchmark shave memcpy    
PIPE:LOS: Shave 0 will transfer memory via CPU
PIPE:LOS:   1KB DDR to DDR  496.5572 MB/s
PIPE:LOS:   1KB CMX to DDR 1144.4092 MB/s
PIPE:LOS:   1KB DDR to CMX  748.3237 MB/s
PIPE:LOS:   1KB CMX to CMX 2615.7925 MB/s
PIPE:LOS:  32KB DDR to DDR  558.0025 MB/s
PIPE:LOS:  32KB CMX to DDR 1297.1290 MB/s
PIPE:LOS:  32KB DDR to CMX 1105.0863 MB/s
PIPE:LOS:  32KB CMX to CMX 3947.3684 MB/s
PIPE:LOS: 128KB DDR to DDR  558.9673 MB/s
PIPE:LOS: 128KB CMX to DDR 1298.2067 MB/s
PIPE:LOS: 128KB DDR to CMX 1110.3051 MB/s
PIPE:LOS: 128KB CMX to CMX 3996.5894 MB/s
PIPE:LOS: 
PIPE:LOS: 2. Benchmark shave DMA copy  
PIPE:LOS: Shave 0 will transfer memory via DMA
PIPE:LOS:   1KB DDR to DDR 1042.5934 MB/s
PIPE:LOS:   1KB CMX to DDR 1233.5526 MB/s
PIPE:LOS:   1KB DDR to CMX  994.8005 MB/s
PIPE:LOS:   1KB CMX to CMX 1171.8750 MB/s
PIPE:LOS:  32KB DDR to DDR 1759.4069 MB/s
PIPE:LOS:  32KB CMX to DDR 4206.8657 MB/s
PIPE:LOS:  32KB DDR to CMX 3633.0166 MB/s
PIPE:LOS:  32KB CMX to CMX 4210.6445 MB/s
PIPE:LOS: 128KB DDR to DDR 1802.4945 MB/s
PIPE:LOS: 128KB CMX to DDR 4383.6577 MB/s
PIPE:LOS: 128KB DDR to CMX 3875.3682 MB/s
PIPE:LOS: 128KB CMX to CMX 4477.3447 MB/s
PIPE:LOS: 
PIPE:LOS: 3. Benchmark multiple SHAVEs DMA 
PIPE:LOS: Multiple shave processors transfer memory simultaneously via DMA
PIPE:LOS:  1 SHAVEs 16KB CMX to DDR 3304.36 MB/s
PIPE:LOS:  1 SHAVEs 16KB DDR to CMX 3852.51 MB/s
PIPE:LOS:  2 SHAVEs 16KB CMX to DDR 4466.93 MB/s
PIPE:LOS:  2 SHAVEs 16KB DDR to CMX 4442.12 MB/s
PIPE:LOS:  4 SHAVEs 16KB CMX to DDR 4576.31 MB/s
PIPE:LOS:  4 SHAVEs 16KB DDR to CMX 4383.08 MB/s
PIPE:LOS:  8 SHAVEs 16KB CMX to DDR 4574.25 MB/s
PIPE:LOS:  8 SHAVEs 16KB DDR to CMX 4388.08 MB/s
PIPE:LOS: 12 SHAVEs 16KB CMX to DDR 4579.38 MB/s
PIPE:LOS: 12 SHAVEs 16KB DDR to CMX 4444.70 MB/s
PIPE:LOS:  1 SHAVEs 32KB CMX to DDR 3602.47 MB/s
PIPE:LOS:  1 SHAVEs 32KB DDR to CMX 4174.65 MB/s
PIPE:LOS:  2 SHAVEs 32KB CMX to DDR 4447.15 MB/s
PIPE:LOS:  2 SHAVEs 32KB DDR to CMX 4419.28 MB/s
PIPE:LOS:  4 SHAVEs 32KB CMX to DDR 4485.56 MB/s
PIPE:LOS:  4 SHAVEs 32KB DDR to CMX 4390.66 MB/s
PIPE:LOS:  8 SHAVEs 32KB CMX to DDR 4552.45 MB/s
PIPE:LOS:  8 SHAVEs 32KB DDR to CMX 4249.00 MB/s
PIPE:LOS: 12 SHAVEs 32KB CMX to DDR 4568.92 MB/s
LOS: LeonOS (P0:ALOS) suspended at 0x701A032C (Application terminated successfully)
PIPE:LOS: 12 SHAVEs 32KB DDR to CMX 4246.02 MB/s
PIPE:LOS: All transactions were verified and passed
PIPE:LOS: All benchmarks finished
PIPE:LOS: 
PIPE:LOS: 
