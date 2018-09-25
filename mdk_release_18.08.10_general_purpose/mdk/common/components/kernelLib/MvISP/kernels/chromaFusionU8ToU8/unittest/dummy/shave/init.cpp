///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     chromaFusionU8ToU8 kernel function call for unitary test
///

#include "chromaFusionU8ToU8.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

// 16 padding values added to the maximum width
#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING


u8  inCRshort[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8  inCGshort[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8  inCBshort[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8   inCRlong[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8   inCGlong[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8   inCBlong[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
half  inAlpha[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputR_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data")))  outputR[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputR_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputG_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data")))  outputG[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputG_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputB_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data")))  outputB[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputB_u32postPad[4] __attribute__ ((aligned (16)));



int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
		mvispChromaFusionU8ToU8(inCRshort, inCGshort, 
                     inCBshort, inCRlong, inCGlong, 
                     inCBlong, inAlpha, outputR, outputG,
                     outputB, width);
   #else
		mvispChromaFusionU8ToU8_asm(inCRshort, inCGshort, 
                     inCBshort, inCRlong, inCGlong, 
                     inCBlong, inAlpha, outputR, outputG,
                     outputB, width);
	#endif
		
	SHAVE_HALT; 
    return 0;
}
