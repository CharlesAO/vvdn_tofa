///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    	scale05BilinHV_U8ToU8 kernel function call for unitary test
///

#include <scale05BilinHV_U8ToU8.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include "svuCommonShave.h"
#include <mv_types.h>

#define OUT_PADDING 16
#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2
u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH * 2] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH + 2 * OUT_PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u8 *inLine[TEST_FRAME_HEIGHT];
u8 **inLines;

int main( void )
{

	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
	}
	inLines = (u8**)inLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvispScale05BilinHV_U8ToU8(inLines, &output[OUT_PADDING], width);
	#else
	mvispScale05BilinHV_U8ToU8_asm(inLines, &output[OUT_PADDING], width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
