Active baseline power evaluation

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This example shows the DDR, DMA and static power consumption.

Software description
=======================
This example measure the temperature (only for {Myriad_version} chip version) and
the power consumption for three states:
1. static power with minimal clocks, with DDR disabled.
2. Static power with minimal clocks, with DDR enabled.
3. static power with DMA transactions. All transfers are checked.
The sampling of 4 rails of current take ~ 5 ms and the dma transfer takes ~6.67 ms.
So, to measure all rails, we need to repeat the transfer for 4 times.
DMA transfer rate is limited to 12000 MB/s because the transfer should
take more than 5 ms.

For each state, all the current rails are measured and power consumption is displayed.
For the consistency of the measurementes, the measurements are done for ten times.

Hardware needed
==================
Myriad2 -This software should run on MV182, MV212 boards.
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
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
The results  consist in printf seen using the debugger:
For each state, are printed the power and temperature.
The power numbers will vary slightly.

Ma2450
=======================
UART: Board Mv0212 initialized, revision = 5
UART: Static power measurement with minimal clocks (no DDR)
UART: Core mW = 142.290, DDR mW =   4.620, Temperature CSS=40.7C, MSS=39.5C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 141.240, DDR mW =   4.620, Temperature CSS=40.7C, MSS=39.0C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 140.790, DDR mW =   4.620, Temperature CSS=40.7C, MSS=39.0C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 140.790, DDR mW =   4.620, Temperature CSS=40.7C, MSS=38.8C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 141.240, DDR mW =   4.620, Temperature CSS=40.7C, MSS=38.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 140.790, DDR mW =   4.620, Temperature CSS=40.7C, MSS=38.4C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 140.790, DDR mW =   4.620, Temperature CSS=40.7C, MSS=38.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 140.790, DDR mW =   4.620, Temperature CSS=40.7C, MSS=39.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 141.240, DDR mW =   4.620, Temperature CSS=40.7C, MSS=38.6C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 141.240, DDR mW =   4.620, Temperature CSS=40.7C, MSS=39.0C, UPA0=39.0C, UPA1=39.7C
UART:
UART: Static power measurement with minimal clocks (DDR)
UART: Core mW = 277.290, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.290, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.0C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.290, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.0C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 278.790, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.4C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.515, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.515, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.2C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.965, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.8C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.515, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.6C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.515, DDR mW =  49.710, Temperature CSS=40.7C, MSS=39.5C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 277.515, DDR mW =  49.710, Temperature CSS=40.7C, MSS=38.4C, UPA0=39.0C, UPA1=39.7C
UART:
UART: Static power measurement with DMA transactions
UART: Core mW = 358.139, DDR mW = 324.180, Temperature CSS=40.7C, MSS=38.6C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.540, Temperature CSS=40.7C, MSS=39.7C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 357.959, DDR mW = 324.750, Temperature CSS=40.7C, MSS=39.9C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.570, Temperature CSS=40.7C, MSS=39.9C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.810, Temperature CSS=40.7C, MSS=39.7C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.330, Temperature CSS=40.7C, MSS=39.5C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.150, Temperature CSS=40.7C, MSS=39.0C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.900, Temperature CSS=40.7C, MSS=40.1C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 324.720, Temperature CSS=40.7C, MSS=39.5C, UPA0=39.0C, UPA1=39.7C
UART: Core mW = 359.450, DDR mW = 325.050, Temperature CSS=40.7C, MSS=39.2C, UPA0=39.0C, UPA1=39.7C
UART:
UART: moviUnitTest:PASSED


Ma2150
=======================
UART: Static power measurement with minimal clocks (no DDR)
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.5C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=33.0C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 147.000, DDR mW =   2.400, Temperature CSS=33.2C, MSS=32.1C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=33.0C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.2C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.9C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=32.3C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.5C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.7C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 146.910, DDR mW =   2.400, Temperature CSS=33.2C, MSS=31.9C, UPA0=32.1C, UPA1=32.6C
UART:
UART: Static power measurement with minimal clocks (DDR)
UART: Core mW = 239.910, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.2C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 239.910, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.9C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.7C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=32.1C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=32.8C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.9C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.7C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=32.6C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=32.8C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 241.410, DDR mW =  34.260, Temperature CSS=33.2C, MSS=31.9C, UPA0=32.1C, UPA1=32.6C
UART:
UART: Static power measurement with DMA transactions
UART: Core mW = 307.691, DDR mW = 164.940, Temperature CSS=33.2C, MSS=32.6C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 310.574, DDR mW = 165.390, Temperature CSS=33.2C, MSS=34.3C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 312.061, DDR mW = 164.940, Temperature CSS=33.2C, MSS=33.7C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 312.061, DDR mW = 165.420, Temperature CSS=33.2C, MSS=35.4C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 312.061, DDR mW = 164.940, Temperature CSS=33.2C, MSS=35.6C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 312.061, DDR mW = 165.540, Temperature CSS=33.2C, MSS=35.0C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 312.061, DDR mW = 165.060, Temperature CSS=33.2C, MSS=36.5C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 313.547, DDR mW = 165.120, Temperature CSS=33.2C, MSS=36.3C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 313.547, DDR mW = 165.960, Temperature CSS=33.2C, MSS=35.6C, UPA0=32.1C, UPA1=32.6C
UART: Core mW = 313.547, DDR mW = 165.600, Temperature CSS=33.2C, MSS=36.0C, UPA0=32.1C, UPA1=32.6C
UART:
UART: moviUnitTest:PASSED


Note
==================
The example contains specific power reduction techniques which makes that
"LRT: Leon entered error mode." message appear. It is normal behaviour, because the LRT
isn't used at all and we restart MSS clocks (which contain clock for LRT).
 MSS_MXI_CTRL clock (from MSS) is used when DMA transactiuons are performed.
