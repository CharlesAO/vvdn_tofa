///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     AddV2Fp16 kernel function call for unitary test
///

#include "addV2Fp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define PADDING 32
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3



half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));


/*output pre pad marker*/
half  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
half __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width __attribute__ ((aligned (16)));;
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i];
		outLine[i] = (half*)output[i];

	}

	inLines = (half**)inLine;
	outLines = (half**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvAddV2Fp16(outLines, inLines, width);
	#else
		mvcvAddV2Fp16_asm(outLines, inLines, width);
	#endif
	
	SHAVE_HALT;
    return 0;
}
