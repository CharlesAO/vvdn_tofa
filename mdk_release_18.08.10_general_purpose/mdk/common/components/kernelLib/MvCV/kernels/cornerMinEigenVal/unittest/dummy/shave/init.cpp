///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CornerMinEigenVal kernel function call for unitary test
///

#include "cornerMinEigenVal.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 4 (kernel_size - 1) padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 16

u8 __attribute__ ((aligned (16))) buffer[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));;

u8 __attribute__ ((aligned (16))) input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];


u8 *interim;
u32 width;
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8** inLines;
u8** outLines;



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i]+8;
		outLine[i] = (u8*)output[i];
	}

	inLines = (u8**)inLine;
	outLines = (u8**)outLine;
	interim = (u8*) buffer;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCornerMinEigenVal(inLines, outLines, interim,  width);
	#else	
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvCornerMinEigenVal_opt(inLines, outLines, interim,  width);
		#else
		mvcvCornerMinEigenVal_asm(inLines, outLines, interim,  width);
		#endif
	#endif

	SHAVE_HALT; 
    return 0;
}
