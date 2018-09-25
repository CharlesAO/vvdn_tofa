///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     convSeparable5x5Fp16ToFp16 kernel function call for unitary test
///

#include "convSeparable5x5Fp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 16 padding values added to the maximum width
#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 16

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half conv[3] __attribute__ ((aligned (16)));

half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;

int main( void )
{
	for(int i = 0; i < 5/*TEST_FRAME_HEIGHT*/; i++)
	{
		inLine[i] = (half*)&input[i][8];
		outLine[i] = (half*)output[i];
	}

	inLines = (half**)inLine;
	outLines = (half**)outLine;

 
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvSeparable5x5Fp16ToFp16(outLines, inLines, conv, width);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvConvSeparable5x5Fp16ToFp16_opt(outLines, inLines, conv, width);
		#else
		mvcvConvSeparable5x5Fp16ToFp16_asm(outLines, inLines, conv, width);
		
		#endif
	#endif
	
	SHAVE_HALT; 
    return 0;
}
