///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPacker10b kernel function call for unitary test
///

#include <pixelPacker10b.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920

u16 input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output32_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output32[TEST_FRAME_WIDTH/4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output32_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output8_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output8[TEST_FRAME_WIDTH/4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output8_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));


int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispPixelPacker10b(input, (u32*)output32, output8, width);
	#else
	mvispPixelPacker10b_asm(input, (u32*)output32, output8, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
