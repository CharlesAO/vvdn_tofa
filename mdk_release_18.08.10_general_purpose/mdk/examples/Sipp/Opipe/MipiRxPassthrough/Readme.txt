MipiRxPassthrough

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This test uses SIPP TX0->RX1 in loopback mode (passthrough config)
On the resources side, this application uses: LOS, LRT and Sipp HW.

The LOS is used just to start the relevant clocks and the LRT which runs the opipe code.
The MIPI Rx/TX filters are included in SIPP for direct connection to MIPI Rx or Tx controllers.
The filters connect to the MIPI controllers via parallel interfaces and are used to stream data into/out of CMX  directly from/to the SoC’s MIPI Controller. See the SIPP_accelerators doc for further details.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 board.

Build
==================
Please type "make help" if you want to learn available targets.

Myriad2 silicon - To build the application:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Setup
==================

Myriad2 silicon - To run the application:
    - open terminal and type make "start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary 
to specify the MV_SOC_REV in the terminal.

Expected output
==================
DEBUG: unitTestVerbosity()    : VERBOSITY_QUIET

DEBUG: loadMemFromFile()      : o:000000 l:131072 -> 0x70044008 [lowLight_512x128.raw] 
DEBUG: dumpMemoryToFile()     : (70004008 l:00040000)[out.raw]

DEBUG: unitTestMemCompare()   : (70004008 vs 70044008 l:00020000)             => PASS
DEBUG: dumpMemoryToFile()     : (70024008 l:00040000)[out.raw]

DEBUG: unitTestMemCompare()   : (70024008 vs 70044008 l:00020000)             => PASS
DEBUG: 
DEBUG: moviUnitTest:PASSED

