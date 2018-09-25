///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    random noise fp16 kernel function call for unitary test
///

#include <randNoiseFp16.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>
#include <moviVectorTypes.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 1

half input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width  __attribute__ ((aligned (16)));
float noiseStrength  __attribute__ ((aligned (16)));

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispRandNoiseFp16(output, input, noiseStrength, width);
	#else
	mvispRandNoiseFp16_asm(output, input, noiseStrength, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
