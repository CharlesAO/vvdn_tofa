///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    	scale2xBilinHV075_U16ToU16 kernel function call for unitary test
///

#include <scale2xBilinHV075_U16ToU16.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include "svuCommonShave.h"
#include <mv_types.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 4

u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH/2 + PADDING] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u16 *inLine[TEST_FRAME_WIDTH/2 + PADDING];
u16 **inLines;
u16 *outLine[TEST_FRAME_WIDTH];
u16 **outLines;

int main( void )
{

	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u16*)input[i] + PADDING / 2;
		outLine[i] = (u16*)output[i];
	}
	inLines = (u16**)inLine;
	outLines = (u16**)outLine;

#ifdef UNIT_TEST_USE_C_VERSION
	mvispScale2xBilinHV075_U16ToU16(inLines, outLines, width);
#else
	mvispScale2xBilinHV075_U16ToU16_asm(inLines, outLines, width);
#endif 
  
	SHAVE_HALT; 
  return 0;
}
