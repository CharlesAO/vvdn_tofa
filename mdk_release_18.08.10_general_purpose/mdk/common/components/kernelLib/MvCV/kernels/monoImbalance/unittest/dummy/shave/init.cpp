///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MonoImbalance kernel function call for unitary test
///

#include "monoImbalance.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

// 8 padding values added to the maximum width
#define PADDING 8
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 16


u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
int thrDark __attribute__ ((aligned (16)));
int thrBright __attribute__ ((aligned (16)));
int inputBits __attribute__ ((aligned (16)));

u16* inLine[TEST_FRAME_HEIGHT];
half* outLine[TEST_FRAME_HEIGHT];
u16** inLines;
half** outLines;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i]	= (u16*)input[i]  + PADDING / 2;
		outLine[i] 	= (half*)output[i];
	}

	inLines		= (u16**)inLine;
	outLines 	= (half**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMonoImbalance(inLines, outLines,thrDark,thrBright,inputBits, width);
	#else
	mvcvMonoImbalance_asm(inLines, outLines, thrDark,thrBright,inputBits,width);
	#endif
	
	
	
	
	
	
	SHAVE_HALT; 
    return 0;
}
