///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution1x7 fp16 kernel function call for unitary test
///

#include "convolution1x7Fp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING

half input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data")))  __attribute__ ((aligned (16))) output[TEST_FRAME_WIDTH];
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
half conv[7];


int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvolution1x7Fp16ToFp16(&input[8], output, conv, width);
	#else
	mvcvConvolution1x7Fp16ToFp16_asm(&input[8], output, conv, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
