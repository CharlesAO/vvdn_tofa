///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "minKernel.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 1920



 u8 __attribute__ ((aligned (16))) input[TEST_LINE_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) min_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) min[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) min_u32postPad[4] __attribute__ ((aligned (16)));

u32 __attribute__ ((aligned (16))) width;

int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMinKernel(input, min, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvMinKernel_opt(input, min, width);
		#else
		mvcvMinKernel_asm(input, min, width);
		#endif
	#endif

	SHAVE_HALT;
    return 0;
}
