///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "minKernel64.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 1920
#define PADDING 16 
#define OFFSET PADDING/2 



u8 __attribute__ ((aligned (16))) input[64];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) min_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) min[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) min_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvMinKernel64(input, min);
	#else
		mvcvMinKernel64_asm(input, min);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
