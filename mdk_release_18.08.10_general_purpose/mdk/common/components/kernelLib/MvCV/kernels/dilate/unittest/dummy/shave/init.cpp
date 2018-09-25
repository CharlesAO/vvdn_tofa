///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dilate source code
///

#include "dilate.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 8
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 7
#define KERNEL_SIZE 8

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];
u8 kernel[KERNEL_SIZE ][KERNEL_SIZE];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8* kernelLine[KERNEL_SIZE];
u8** src;
u8** dst;
u8** kernelLines;
u32 width;
u32 height;
u32 k;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
		outLine[i] = (u8*)output[i];
	}
	for(int i = 0; i < KERNEL_SIZE; i++)
	{
		kernelLine[i] = kernel[i];
	}

	src = (u8**)inLine;
	dst = (u8**)outLine;
	kernelLines = (u8**)kernelLine;
		
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvDilate(src, dst, kernelLines, width, height, k);
	#else
	mvcvDilate_asm(src, dst, kernelLines, width, height, k);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
