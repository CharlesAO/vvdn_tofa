CmxDmaTransfers

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
Simple application showing how to use CMX DMA API on each processor.

Software description
=======================
Each processor is starting a series of DMA transfers and verifies if the transfers succeeded.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 boards.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary to specify the MV_SOC_REV in the terminal.

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
The results  consist in printf seen using the debugger:

PIPE:LOS: Running CMX DMA transfers on LOS
PIPE:LOS:
PIPE:LOS: CMX DMA simple transfer: transfer completed successfully
PIPE:LOS: CMX DMA simple stride transfer: transfer completed successfully
PIPE:LRT: Running CMX DMA transfers on LRT
PIPE:LRT:
PIPE:LRT: CMX DMA simple transfer: transfer finished successfully
PIPE:LRT: CMX DMA simple stride transfer: transfer completed successfully
PIPE:LOS: CMX DMA list transfer: transfer completed successfully
PIPE:LRT: CMX DMA list transfer: transfer completed successfully
PIPE:LRT: Running CMX DMA transfers on SHAVEs
PIPE:LRT:
PIPE:LRT: Simple transfer on SHAVEs
PIPE:LRT: SHAVE: 0: transfer completed successfully
PIPE:LRT: SHAVE: 1: transfer completed successfully
PIPE:LRT: SHAVE: 2: transfer completed successfully
PIPE:LRT: SHAVE: 3: transfer completed successfully
PIPE:LRT: SHAVE: 4: transfer completed successfully
PIPE:LRT: SHAVE: 5: transfer completed successfully
PIPE:LRT: SHAVE: 6: transfer completed successfully
PIPE:LRT: SHAVE: 7: transfer completed successfully
PIPE:LRT: SHAVE: 8: transfer completed successfully
PIPE:LRT: SHAVE: 9: transfer completed successfully
PIPE:LRT: SHAVE: 10: transfer completed successfully
PIPE:LRT: SHAVE: 11: transfer completed successfully
PIPE:LRT:
PIPE:LRT: Stride transfer on SHAVEs
PIPE:LRT: SHAVE: 0: transfer completed successfully
PIPE:LRT: SHAVE: 1: transfer completed successfully
PIPE:LRT: SHAVE: 2: transfer completed successfully
PIPE:LRT: SHAVE: 3: transfer completed successfully
PIPE:LRT: SHAVE: 4: transfer completed successfully
PIPE:LRT: SHAVE: 5: transfer completed successfully
PIPE:LRT: SHAVE: 6: transfer completed successfully
PIPE:LRT: SHAVE: 7: transfer completed successfully
PIPE:LRT: SHAVE: 8: transfer completed successfully
PIPE:LRT: SHAVE: 9: transfer completed successfully
PIPE:LRT: SHAVE: 10: transfer completed successfully
PIPE:LRT: SHAVE: 11: transfer completed successfully
PIPE:LRT:
PIPE:LRT: List transfer on SHAVEs
PIPE:LRT: SHAVE: 0: transfer completed successfully
PIPE:LRT: SHAVE: 1: transfer completed successfully
PIPE:LRT: SHAVE: 2: transfer completed successfully
PIPE:LRT: SHAVE: 3: transfer completed successfully
PIPE:LRT: SHAVE: 4: transfer completed successfully
PIPE:LRT: SHAVE: 5: transfer completed successfully
PIPE:LRT: SHAVE: 6: transfer completed successfully
PIPE:LRT: SHAVE: 7: transfer completed successfully
PIPE:LRT: SHAVE: 8: transfer completed successfully
PIPE:LRT: SHAVE: 9: transfer completed successfully
PIPE:LRT: SHAVE: 10: transfer completed successfully
PIPE:LRT: SHAVE: 11: transfer completed successfully
