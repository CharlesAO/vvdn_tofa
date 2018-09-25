///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     boxfilter3x3 kernel function call for unitary test
///

#include "boxFilter3x3.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

// 4 (kernel_size - 1) padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 3


u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 normalize;
u32 width;
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8** inLines;
u8** outLines;



int main( void )
{
	inLine[0] = (u8*)input[0] + 8;
	inLine[1] = (u8*)input[1] + 8;
	inLine[2] = (u8*)input[2] + 8;

	outLine[0] = (u8*)output[0];

	inLines = (u8**)inLine;
	outLines = (u8**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvBoxfilter3x3(inLines, outLines, normalize, width);
	#else
	mvcvBoxfilter3x3_asm(inLines, outLines, normalize, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
