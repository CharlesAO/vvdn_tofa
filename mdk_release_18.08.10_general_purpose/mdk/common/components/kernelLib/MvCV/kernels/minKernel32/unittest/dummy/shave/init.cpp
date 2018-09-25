///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "minKernel32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


u8 __attribute__ ((aligned (16))) input[32];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) min_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) min[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) min_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvMinKernel32(input, min);
	#else
		mvcvMinKernel32_asm(input, min);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
