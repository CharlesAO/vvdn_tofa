///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     HarrisResponse kernel function call for unitary test
///

#include <harrisResponse.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 8
#define TEST_FRAME_HEIGHT 8

u32 x;
u32 y;
u32 step_width;
fp32 k;
fp32 output;
u8 input[TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT];




int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	output = mvcvHarrisResponse(input, x, y, step_width, k); 
	#else
	output = mvcvHarrisResponse_asm(input, x, y, step_width, k); 
	#endif
	
	SHAVE_HALT;
	return 0;
}
