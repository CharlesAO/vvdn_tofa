
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     absoluteDiff kernel function call for unitary test
///

#include "convertFp16ToQ4.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920

half input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvertFp16ToQ4(input, output, width);
	#else
	mvcvConvertFp16ToQ4_asm(input, output, width);
	#endif
	
	SHAVE_HALT;
    return 0;
}
