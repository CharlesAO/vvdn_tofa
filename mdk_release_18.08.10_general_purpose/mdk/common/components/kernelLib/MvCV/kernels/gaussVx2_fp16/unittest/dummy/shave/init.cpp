///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief      kernel function call for unitary test
///

#include <gaussVx2_fp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920 
#define TEST_FRAME_HEIGHT 16

half __attribute__ ((aligned (16))) input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;

half* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
half** inLines;
half** outLines;



int main( void )
{
	int i;
	
	for(i = 0; i < 5; i++)
	{
		inLine[i] = (half*)input[i];
		
	}
	outLine[0] = (half*)output[0];
	inLines    = (half**)inLine;
	outLines   = (half**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvGaussVx2_fp16(inLines, outLines, width);
	#else
	mvcvGaussVx2_fp16_asm(inLines, outLines, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
