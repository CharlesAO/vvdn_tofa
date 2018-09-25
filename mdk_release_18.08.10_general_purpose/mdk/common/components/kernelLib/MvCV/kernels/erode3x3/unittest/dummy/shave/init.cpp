///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Erode3x3 kernel function call for unitary test
///

#include <erode3x3.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define PADDING 16
#define TEST_FRAME_WIDTH 1920 + PADDING
#define TEST_FRAME_HEIGHT 3
#define KERNEL_SIZE 4

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u8 kernel[KERNEL_SIZE][KERNEL_SIZE];// = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };

u32 width;
u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8* kernelLine[KERNEL_SIZE];
u8** src;	//inLines
u8** dst;	//outLines
u8** kernelLines;

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i] + PADDING / 2;
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
	mvcvErode3x3(src, dst, kernelLines, width);
	#else
	mvcvErode3x3_asm(src, dst, kernelLines, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
