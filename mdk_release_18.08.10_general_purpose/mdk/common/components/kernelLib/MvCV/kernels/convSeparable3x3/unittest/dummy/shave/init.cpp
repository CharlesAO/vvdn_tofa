///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution Separable 3x3 kernel function call for unitary test
///

#include "convSeparable3x3.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 3

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;;
float conv[2];
u8* inLines[TEST_FRAME_HEIGHT];

__attribute__((dllexport)) int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLines[i] = (u8*)&input[i][8];
	}
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvSeparable3x3(output, inLines, conv, width);
	#else
	mvcvConvSeparable3x3_asm(output, inLines, conv, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
