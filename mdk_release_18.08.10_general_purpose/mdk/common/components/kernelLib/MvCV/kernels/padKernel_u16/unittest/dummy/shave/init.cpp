///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     padKernel_u16 kernel function call for unitary test
///

#include "padKernel_u16.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define MAX_PADDING 32

u16 iBuf[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH + MAX_PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 iBufLen __attribute__ ((aligned (16)));
u32 padSz __attribute__ ((aligned (16)));
u32 padMode __attribute__ ((aligned (16)));
u32 padType __attribute__ ((aligned (16)));
u16 pixValue __attribute__ ((aligned (16)));

int main( void )
{		

#ifdef UNIT_TEST_USE_C_VERSION
	mvcvPadKernel_u16(iBuf, iBufLen, output, padSz, padMode, padType, pixValue);
#else
	mvcvPadKernel_u16_asm(iBuf, iBufLen, output, padSz, padMode, padType, pixValue);
#endif	
	
	SHAVE_HALT; 
    return 0;
}
