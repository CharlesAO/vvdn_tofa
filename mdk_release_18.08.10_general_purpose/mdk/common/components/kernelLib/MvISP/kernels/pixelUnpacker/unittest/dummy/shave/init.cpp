///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     pixelUnpack kernel function call for unitary test
///

#include <pixelUnpacker.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>


#define TEST_FRAME_WIDTH 1920

u32 input32[TEST_FRAME_WIDTH] __attribute__ ((aligned (32)));
u8 input8[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u16 output[TEST_FRAME_WIDTH * 4] __attribute__ ((aligned (16)));

u32 width;
u32 shift;

int main( void )
{
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispPixelUnpacker(input32, input8, output, width, shift);
	#else
	mvispPixelUnpacker_asm(input32, input8, output, width, shift);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
