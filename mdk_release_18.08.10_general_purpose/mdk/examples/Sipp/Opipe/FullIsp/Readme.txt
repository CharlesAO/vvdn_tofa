FullIsp

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The example shows a full isp. 
On the resources side, this application uses: LOS, LRT, Sipp HW, and CMX DMA.
The LOS is used just to start the relevant clocks the the LRT which runs the code that controls the opipe. 
The CMX DMA driver has been used in the Opipe component.

FUll ISP
The filters comprising the oPipe are organized into several pipeline sub-sections: the RAW section, the Luma section, the Chroma section and the RGB section.
For more details pleasee see the Configuring the oPipe section from 07_07_SIPP_accelerators.odt)
By using these filters: Sigma , RAW, Bayer demosaicing, DOG ,Sharpening, Chroma gen, Median, Chroma, Color combination and LUT a full isp is built. 

Hardware needed
==================
Myriad2 - This software should run on MV182 board.

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
DEBUG: loadMemFromFile()      : o:000000 l:131072 -> 0x70118000 [lowLight_512x128.raw]
DEBUG: dumpMemoryToFile()     : (700CD000 l:00019000)[out_P444.y]

DEBUG: dumpMemoryToFile()     : (700A7800 l:0000C800)[out_P444.uv]

DEBUG: dumpMemoryToFile()     : (700E6000 l:00019000)[out_P444.y]

DEBUG: dumpMemoryToFile()     : (700B4000 l:0000C800)[out_P444.uv]

DEBUG: dumpMemoryToFile()     : (700FF000 l:00019000)[out_P444.y]

DEBUG: dumpMemoryToFile()     : (700C0800 l:0000C800)[out_P444.uv]

DEBUG: unitTestMemCompare()   : (700E6000 vs 700CD000 l:00019000)             => PASS
DEBUG: unitTestMemCompare()   : (700B4000 vs 700A7800 l:0000C800)             => PASS
DEBUG: unitTestMemCompare()   : (700FF000 vs 700CD000 l:00019000)             => PASS
DEBUG: unitTestMemCompare()   : (700C0800 vs 700A7800 l:0000C800)             => PASS
DEBUG: 
DEBUG: moviUnitTest:PASSED

