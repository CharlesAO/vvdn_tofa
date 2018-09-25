///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///              convertYUV400ToYUV422 kernel function call for unitary test
/// @brief     ///This kernel converts from yuv400 to yuv422 
///

#include "convertYUV400ToYUV422.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING


u8  __attribute__ ((aligned (16)))  input[TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;



int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvertYUV400ToYUV422(input, output + PADDING/2, width);
	#else
	mvcvConvertYUV400ToYUV422_asm(input, output + PADDING/2, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
