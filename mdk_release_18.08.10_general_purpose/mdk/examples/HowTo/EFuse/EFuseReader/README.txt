EFuse Reader example

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The EFuse high-level API is used to read and display the values the EFuses.
Additionally, an example of a field defined within the customer-use EFuse area is provided.

Software description
=======================
An example of using the EFuse high-level API to read EFuse values.

Hardware needed
==================
Myriad2 -This software should run on MV182 or MV212 boards.

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
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The results consist of printf messages seen using the debugger.


The message displayed should be similar to:
UART: IFSEL:  0
UART: IFCFG:  0x0000000000000000
UART: Serial: 00-00-00-00-00-00-00-00
UART:
UART: Dumping entire EFuse contents
UART:   [  31:   0] : 0x00000000
UART:   [  63:  32] : 0x00000000
UART:   [  95:  64] : 0x00000000
UART:   [ 127:  96] : 0x00000000
UART:   [ 159: 128] : 0x00000000
UART:   [ 191: 160] : 0x00000000
UART:   [ 223: 192] : 0x00000000
UART:   [ 255: 224] : 0x00000000
UART:   [ 287: 256] : 0x00000000
UART:   [ 319: 288] : 0x00000000
UART:   [ 351: 320] : 0x00000000
UART:   [ 383: 352] : 0x00000000
UART:   [ 415: 384] : 0x00000000
UART:   [ 447: 416] : 0x00000000
UART:   [ 479: 448] : 0x00000000
UART:   [ 511: 480] : 0x00000000
UART:   [ 543: 512] : 0x00000000
UART:   [ 575: 544] : 0x00000000
UART:   [ 607: 576] : 0x00000000
UART:   [ 639: 608] : 0x00000000
UART:   [ 671: 640] : 0x00000000
UART:   [ 703: 672] : 0x00000000
UART:   [ 735: 704] : 0x00000000
UART:   [ 767: 736] : 0x00000000
UART:   [ 799: 768] : 0x00000000
UART:   [ 831: 800] : 0x00000000
UART:   [ 863: 832] : 0x00000000
UART:   [ 895: 864] : 0x00000000
UART:   [ 927: 896] : 0x00000000
UART:   [ 959: 928] : 0x00000000
UART:   [ 991: 960] : 0x00000000
UART:   [1023: 992] : 0x00000000

Actual values will depend on the values in the EFuses.
