///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution7x1Fp16ToFp16 kernel function call for unitary test
///

#include "convolution7x1Fp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 7

half  __attribute__ ((aligned (16))) input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half conv[7];

half* inLine[TEST_FRAME_HEIGHT];
half** inLines;



int main( void )
{

	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] 	= (half*)input[i];
	}

	inLines = (half**)inLine;	
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvolution7x1Fp16ToFp16(inLines, output, conv, width);
	#else
	mvcvConvolution7x1Fp16ToFp16_asm(inLines, output, conv, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
