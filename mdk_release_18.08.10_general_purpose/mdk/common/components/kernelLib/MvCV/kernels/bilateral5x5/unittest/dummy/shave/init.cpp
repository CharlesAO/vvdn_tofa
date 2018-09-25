///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     bilateral5x5 kernel function call for unitary test
///

#include "bilateral5x5.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 5

u16 input[TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT]  __attribute__ ((aligned (16)));
u16 sigma[TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



u32 width;


int main( void )
{
		
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvBilateral5x5(&input[2*width], sigma, width, output);
	#else
	mvcvBilateral5x5_asm(&input[2*width], sigma, width, output);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
