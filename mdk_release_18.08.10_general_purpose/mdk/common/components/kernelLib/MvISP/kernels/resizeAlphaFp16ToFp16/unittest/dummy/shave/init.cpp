///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     ResizeAlphaFp16ToFp16 kernel function call for unitary test
///

#include "resizeAlphaFp16ToFp16.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

// 16 padding values added to the maximum width
#define PADDING 16
#define TEST_FRAME_WIDTH 3200 + PADDING
#define TEST_FRAME_HEIGHT 5


half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
half* inLine[TEST_FRAME_HEIGHT];
half** inLines;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i]	= (half*)input[i]  + PADDING / 2;
	}
	
	inLines		= (half**)inLine;

	#ifdef UNIT_TEST_USE_C_VERSION
		mvispResizeAlphaFp16ToFp16(inLines, (half*)output, width);
   #else
		mvispResizeAlphaFp16ToFp16_asm(inLines, (half*)output, width);
	#endif
		
	SHAVE_HALT; 
    return 0;
}
