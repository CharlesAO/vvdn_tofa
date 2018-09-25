
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     absoluteDiff kernel function call for unitary test
///

#include "convert_fp16_u8.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

//#define TEST_FRAME_WIDTH 6
//#define TEST_FRAME_HEIGHT 1

//u8 output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
//half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] = {{2, 4, 6, 7, 6, 7}};
//u8 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] = {{1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15}};


#define TEST_FRAME_WIDTH 1920

half input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvert_fp16_u8(input, output, width);
	#else
	mvcvConvert_fp16_u8_asm(input, output, width);
	#endif
	
	SHAVE_HALT;
    return 0;
}
