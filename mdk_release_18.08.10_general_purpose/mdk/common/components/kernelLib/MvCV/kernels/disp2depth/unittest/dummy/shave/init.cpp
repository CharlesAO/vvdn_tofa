///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     absoluteDiff kernel function call for unitary test
///

#include "disp2depth.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define LUT_TABLE_WIDTH 256

u8 __attribute__ ((aligned (16))) input[TEST_FRAME_WIDTH];// = { 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
half __attribute__ ((aligned (16))) lut_table[LUT_TABLE_WIDTH];// = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
u32 flip_disp2depth __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH * 2] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;// = 16;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvDisp2depth(input, (half*)(&output[width]), lut_table, flip_disp2depth, width);
	#else
	mvcvDisp2depth_asm(input, (half*)(&output[width]), lut_table, flip_disp2depth, width);
	#endif
	
	SHAVE_HALT;
    return 0;
}
