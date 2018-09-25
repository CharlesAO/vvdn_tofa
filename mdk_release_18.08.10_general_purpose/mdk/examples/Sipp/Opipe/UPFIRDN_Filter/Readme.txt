UPFIRDN_Filtre

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The SimplePassthrough example uses the UPDFIRDN filter. 
Please see the SIPP_UPFIRDN_CFG section from the SIPP_accelerators doc for further detail regarding the filter used.

On the resources side, this application uses: LOS, LRT, Sipp HW, and CMX DMA.
The LOS is used just to start the relevant clocks and also the LRT which runs the opipe code. 
The CMX DMA driver has been used in the Opipe component.

At setup Leon configures the filter then feeds the filter several lines of data via CMXDMA. When DMA transfer is complete, Leon updates Filter IBFL.
The filter starts to produce data and gradually mark the input data as CONSUMED by decreasing IBFL-level. 
As lines get produced, Dma copies data out to main buffer. In the IBFL-decrement IRQ, Leon triggers CMXDMA to bring some data in the newly released memory buffer, and so on until the enire frame is fed. On the other side, as line get produces, DMA copies data to the ouput buffer.

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 board.

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
DEBUG: loadMemFromFile()      : o:000000 l:016384 -> 0x70020008 [inpU8_256x64.raw] 
DEBUG: loadMemFromFile()      : o:000000 l:065536 -> 0x70000008 [expU8_512x128.raw] 
DEBUG: dumpMemoryToFile()     : (70010008 l:00010000)[out.bin]

DEBUG: unitTestMemCompare()   : (70010008 vs 70000008 l:00010000)             => PASS
DEBUG: 
DEBUG: moviUnitTest:PASSED

