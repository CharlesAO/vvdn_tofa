///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution Separable 3x3 Fp16ToFp16 kernel function call for unitary test
///

#include "convSeparable3x3Fp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 3

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;
half conv[2];
half* inLines[TEST_FRAME_HEIGHT];

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLines[i] = (half*)&input[i][8];
	}

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvSeparable3x3Fp16ToFp16(output, inLines, conv, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvConvSeparable3x3Fp16ToFp16_opt(output, inLines, conv, width);
		#else
		mvcvConvSeparable3x3Fp16ToFp16_asm(output, inLines, conv, width);
		#endif
	#endif
	SHAVE_HALT; 
    return 0;
}
