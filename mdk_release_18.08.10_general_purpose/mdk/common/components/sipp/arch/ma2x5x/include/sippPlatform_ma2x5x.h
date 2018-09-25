///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef __SIPP_PLATFORM_MA2x5x_H__
#define __SIPP_PLATFORM_MA2x5x_H__

#include <mv_types.h>
//////////////////////////////////////////////////////////////
// Name me

#if defined(__sparc) || defined(__MOVICOMPILE__)
  #define INLINE inline
  #define ALIGNED(x) __attribute__((aligned(x)))
  #define SECTION(x) __attribute__((section(x)))
  #ifndef DBG_PRINT
  #define DBG_PRINT(x...)
  #endif

#else // PC world
  #define INLINE
  #define ALIGNED(x)
  #define SECTION(x)
  #ifndef DBG_PRINT
  #define DBG_PRINT printf
  #endif
  #define unitTestFinalReport()
#endif

#if defined(__sparc) && defined(MYRIAD2) //Thus SPARC for Myriad2
 #include  "DrvSippDefines.h"

 #include <registersMyriad.h>    //various registers used under the hood by some functions
 #include <swcLeonUtilsDefines.h> //for NOP
 #include <DrvSvuDefines.h>
 #include <DrvMutex.h>
 #include <DrvSvu.h>
 #include <stdlib.h>
 #include <swcWhoAmI.h>

#if defined(SIPP_VCS)
 #include <UnitTestApi.h>
 #include <VcsHooksApi.h>
#endif

 #include <stdio.h>               //printf
 #include <swcSliceUtils.h>
 #include <swcShaveLoader.h>
 #include <swcMemoryTransfer.h>

 #if defined(SIPP_VCS)
	#if defined(SIPP_NO_PRINTF)
		#define VCS_PRINT_INT(a)        //nothing
	#else
 		#define VCS_PRINT_INT(a) sippPalPrintInt((u32)a)
	#endif
 #else
 #define VCS_PRINT_INT(a)        //nothing
 #endif

 void   sippSetShaveWindow (u32 svuNo, u32 winNo, u32 value);
 void   sippLoadMbinMulti (u8 *mbinImg, u32 firstSlc, u32 lastSlc);

 u32 sippSolveShaveAddr(u32 inAddr, u32 sliceNo);

 #define CONCAT(A,B) A ## B
 #define SVU_SYM(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM0(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM1(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM2(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM3(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM4(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM5(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM6(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SVU_SYM7(s) CONCAT(SS_,s)   // moviLink adds extra "SS_"
 #define SHAVE_MAIN main            //Leon will see something like SS__main through SVU_SYM macro
 #define U32_MEMSET(addr, len, val) swcU32memsetU32((u32*)(addr), (u32)(len), (u32)(val))
 #define U32_MEMCPY(dest, src, len) swcU32memcpy((u32*)(dest), (u32*)(src), (u32)(len))

 #define SIPP_MBIN(x)            (x)

//for test mode, allow DDR buffers to be mapped in CMX to speedup simulations...
 #ifndef DDR_DATA
 #define DDR_DATA     __attribute__((section(".ddr.bss")))
 #endif

 #ifndef CMX_TEXT
 #define CMX_TEXT
 #endif

 #ifndef CMX_DATA
 #define CMX_DATA
 #endif

 void sippWaitHWUnit(u32 id);

 //#############################################################################
#elif defined(__MOVICOMPILE__)
 #include <string.h>
 #include <stdint.h>
 #include <svuCommonShave.h>
 #include <moviVectorTypes.h> //for half data type
 #define SVU_SYM(s) s
 #define SVU_SYM0(s) s
 #define SVU_SYM1(s) s
 #define SVU_SYM2(s) s
 #define SVU_SYM3(s) s
 #define SVU_SYM4(s) s
 #define SVU_SYM5(s) s
 #define SVU_SYM6(s) s
 #define SVU_SYM7(s) s
 #define SHAVE_MAIN main     //on Sabre, this is the main function
 typedef half Half;

 //#############################################################################
#else //PC world

 #define SIPP_PC
 //#define _CRT_SECURE_NO_DEPRECATE //remove some VisualC warnings....
 #include <stdio.h>
 #include <string.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include <math.h>
 #include "sippHw.h"
 #include <half.h>

 #define SVU_SYM(s)              s
 #define SHAVE_MAIN              shave_main

// KMC - change the below to sippPalMemSet
 #define U32_MEMSET(addr, len, val) memset((void*)(addr), (int)(len),   (size_t)(val))
 #define U32_MEMCPY(dest, src, len) memcpy((void*)(dest), (void*)(src), (size_t)(len))

 #define SIPP_MBIN(x)            0

 #define VCS_PRINT_INT(a)        //nothing
 //#define MXI_CMX_BASE_ADR        0x10000000 //what really matters is that lower 24 bits are ZERO

 #define CMX_TEXT   //dummy
 #define CMX_RODATA //dummy
 #define CMX_DATA   //dummy
 #define CMX_BSS    //dummy
 #define DDR_TEXT   //dummy
 #define DDR_RODATA //dummy
 #define DDR_DATA   //dummy
 #define DDR_BSS    //dummy

 extern int scGetShaveNumber(); //

 // Define for unused variables
 #define UNUSED(x) (void)x

 #define NOP
#endif


#if !defined(SIPP_SVU_STACK_SZ)
#define SIPP_SVU_STACK_SZ (8*1024)
#endif


#endif // __SIPP_PLATFORM_MA2x5x_H__
