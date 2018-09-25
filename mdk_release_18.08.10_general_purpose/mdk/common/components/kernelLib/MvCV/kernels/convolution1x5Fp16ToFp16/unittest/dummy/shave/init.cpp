///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution1x5 fp16 kernel function call for unitary test
///

#include "convolution1x5Fp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

// 16 padding values added to the maximum width
#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 1

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width = 16;
half conv[5];
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[1];
half** inLines;
half** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i] + PADDING / 2;
	}
	outLine[0] = (half*)output[0];
	
	inLines = (half**)inLine;
	outLines = (half**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvolution1x5Fp16ToFp16(inLines, outLines, conv, width);
	#else
	mvcvConvolution1x5Fp16ToFp16_asm(inLines, outLines, conv, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
