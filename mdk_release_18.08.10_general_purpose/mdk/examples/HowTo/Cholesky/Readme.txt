Cholesky Solver

Supported Platform
===================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
This example use Cholesky component to solve the Ax=b equation, with A[N][N], x[N] and b[N].
In common/components/Cholesky/README is the  algorithm's description

Hardware needed
==================
Myriad2 -This software should run on any myriad2 chip.
A MV0182 or MV212 board, housing the Myriad2 chip. 

Build
==================
To build the project please type:
"make clean"
"make default MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Setup
==================
Open terminal and type "make start_server" 
Open another terminal and type " ./do.sh N MV_SOC_REV={Myriad_version}"

Note, N has to be a multiple of 4. 

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Appendix A - Tools Description
=================
!!!!!in tools folder 
[ solver ]
$ ./solver -?

This tool generates test vectors for a specific problem size; it produces random data for A and b, in the specified range, using the specified random seed (thus, assuring 100% reproducibility of results), and solves that problem.
-----------------------------------------------------------------------------
[ matdiff ]
$ ./matdiff -?

Simpler tool that lets once compare two vectors or square matrices; for matrices, upper-triangular and lower-triangular comparisons are supported, in addition to normal comparison (which, being the default, doesn't have a switch).
-----------------------------------------------------------------------------
[ printsqm ]
$ ./printsqm -?

A simple tool for printing the contents of a square matrix; in addition to regular matrices, support for printing upper/lower triangular matrices is also probided.

Expected output
==================

Generating testcase data for 100x100...
Solving an n=100 system, RNG seed=126, vals=[-1.00:1.00)

* CHOLESKY solution *
Total operations for Chokesky flow:
  adds=691550 muls=691550 divs=5150 sqrts=100
  scans=0
Normalized error of the Cholesky model (||b-Ax||): 1.192708e-05

Building...
Building 'release' configuration for ma2150
Application ELF       : output/Cholesky.elf
Application MAP File  : ./output/Cholesky.map
Application Linker Script  : config/custom.ldscript
MVCMD boot image      : output/Cholesky.mvcmd
Dbg Listing      : ./output/lst/Cholesky_sparc.lst
Shave Dbg Listing     : ./output/lst/Cholesky_shave.lst
Symbols Listing  : ./output/Cholesky.sym
Finished building 'release' configuration for ma2150
rm output/lst/Cholesky.elf.nodebug

Running...
Application ELF       : output/Cholesky.elf
Application MAP File  : ./output/Cholesky.map
Application Linker Script  : config/custom.ldscript
Movidius Debugger (moviDebug2) v00.88.0 Build 138412
Loading Tcl library...
Loading debug library...
Connecting to 127.0.0.1:30001
Connection      : OK [127.0.0.1:30001 | Olimex ARM-USB-TINY-H @ 3000kHz]
Target Voltage  : OK                    
JTAG IDCODE     : ma2x5x                 [0x1C450661]
Starting TCF agent...
 Loading ../../../common/scripts/debug/mdkTcl/commands/SoC/common/utils.scr
 Loading ../../../common/scripts/debug/mdkTcl/commands/SoC/common/inspectDynamic.scr
 Loading ../../../common/scripts/debug/mdkTcl/commands/SoC/common/inspectWindows.scr
 Loading ../../../common/scripts/debug/mdkTcl/commands/SoC/ma2x5x/common.scr
 Loading ../../../common/scripts/debug/mdkTcl/commands/SoC/ma2x5x/inspectCDMA.scr
Loading application...
Leon halt status: ok
Loading section <.text> start = 0x701A0000 length = 51260 bytes.
Loading section <.text.eh_frame> start = 0x701AC840 length = 128 bytes.
Loading section <.dtors> start = 0x701AC8C0 length = 4 bytes.
Loading section <.rodata> start = 0x701AC8C8 length = 4921 bytes.
Loading section <.init> start = 0x701ADC10 length = 56 bytes.
Loading section <.fini> start = 0x701ADC48 length = 40 bytes.
Loading section <.data> start = 0x701ADC70 length = 1164 bytes.
Loading section <.heapSection> start = 0x701AE100 length = 6144 bytes.
Loading section <S.shv0.cmx.text> start = 0x70000000 length = 4326 bytes.
Loading section <.ddr.data> start = 0x80000000 length = 4 bytes.
Total bytes loaded = 68047.
Loading input data...
Running...
UART: System running at 600.00MHz
UART: Leon running from CMX slice 13; stack: size=0x3D510, top=0x701f7600
UART: 
UART: Buffers: A[100][100] @0x701b04b0..0x701ba0ef ; A, then A'A, then L
UART:          b[100]      @0x701b0320..0x701b04af ; b
UART:          x[100]      @0x701b0190..0x701b031f ; A'b, then L\(A'b),
UART:                                                       then L'\y
UART:          tmp[100]    @0x701afb50..0x701afcdf ; in-place syrk buffer
UART: 
UART: Running a full lineq sequence...
UART: . gemvl_t( x @0x701b0190, A @0x701b04b0, b @0x701b0320, 100, 0x190 )
UART: . syrk_tn_l( A @0x701b04b0, tmp @0x701afb50, 100, 0x190 )
UART: . potrf_ln( A @0x701b04b0, 100, 0x190 );
UART: . trsvl_ln( x @0x701b0190, A @0x701b04b0, x @0x701b0190, 100, 0x190 )
UART: . trsvl_lt( x @0x701b0190, A @0x701b04b0, x @0x701b0190, 100, 0x190 )
UART: All done.
UART: 
UART: Computation took:
UART:   - gemvl_t@opt   :     0.0ms (       9034cycles)
UART:   - syrk_tn_l@opt :     0.4ms (     241739cycles)
UART:   - potrf_ln@opt  :     0.1ms (      84235cycles)
UART:   - trsvl_ln@opt  :     0.0ms (      16089cycles)
UART:   - trsvl_lt@opt  :     0.0ms (      15883cycles)
UART: Total: 0.6ms (366980cycles)
UART: All done.
LOS: LeonOS (P0:ALOS) suspended at 0x701A032C (Application terminated successfully)
Saving output+intermediate data...
All done.
moviDebug2 exiting
Shutting down TCF agent...
TCF agent shutdown complete.
moviDebug2 exited

Comparing matrices (N=100)...
  M: Matrices are binarily identical
  c: Vectors are binarily identical
  L: Matrices different, errors: normalized=2.314788e-08, absolute=0.002153442
  y: Vectors different, errors: normalized=5.933918e-06, absolute=0.0007565822
  x: Vectors different, errors: normalized=0.0008939134, absolute=0.7137344

Timings (N=100):
  gemvl_t@opt   :     0.0ms (       9034cycles)
  syrk_tn_l@opt :     0.4ms (     241739cycles)
  potrf_ln@opt  :     0.1ms (      84235cycles)
  trsvl_ln@opt  :     0.0ms (      16089cycles)
  trsvl_lt@opt  :     0.0ms (      15883cycles)

All done.




