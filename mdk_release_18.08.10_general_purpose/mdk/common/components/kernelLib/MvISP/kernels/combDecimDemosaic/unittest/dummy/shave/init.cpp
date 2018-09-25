///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Combine decimation demosaic kernel function call for unitary test
///

#include "combDecimDemosaic.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 8
#define TEST_FRAME_WIDTH (1920 + PADDING * 2)
#define TEST_FRAME_HEIGHT 4

u16  input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[3][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width __attribute__ ((aligned (16)));

u16* inLine[TEST_FRAME_HEIGHT];
u16** inLines;

u16* outLine[TEST_FRAME_HEIGHT];
u16** outLines;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u16*)(&input[i][PADDING]);
	}
    inLines = (u16**)inLine;
    
    outLine[0] = (u16*)(&output[0][0]);
    outLine[1] = (u16*)(&output[1][0]);
    outLine[2] = (u16*)(&output[2][0]);
	outLines = (u16**)outLine;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispCombDecimDemosaic(outLines, inLines, width);
	#else
	mvispCombDecimDemosaic_asm(outLines, inLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
