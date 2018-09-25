///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    	scale2xBilinHV025_U8ToU8 kernel function call for unitary test
///

#include <scale2xBilinHV025_U8ToU8.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include "svuCommonShave.h"
#include <mv_types.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH/2 + PADDING] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u8 *inLine[TEST_FRAME_HEIGHT];
u8 **inLines;
u8 *outLine[TEST_FRAME_HEIGHT];
u8 **outLines;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i] + PADDING / 2;
		outLine[i] = (u8*)output[i];
	}
	
	inLines = (u8**)inLine;
	outLines = (u8**)outLine;

#ifdef UNIT_TEST_USE_C_VERSION
	mvispScale2xBilinHV025_U8ToU8(inLines, outLines, width);
#else
	mvispScale2xBilinHV025_U8ToU8_asm(inLines, outLines, width);
#endif
	
	SHAVE_HALT; 
    return 0;
}
