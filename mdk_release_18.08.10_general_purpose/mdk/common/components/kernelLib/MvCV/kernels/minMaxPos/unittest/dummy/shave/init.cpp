///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     minMax kernel function call for unitary test
///

#include <minMaxPos.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u32 height __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minVal_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minVal __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minVal_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxVal_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxVal __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxVal_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) minPos_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) minPos __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) minPos_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) maxPos_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) maxPos __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) maxPos_u32postPad[4] __attribute__ ((aligned (16)));

u8* line;
u8 mask[TEST_FRAME_WIDTH * TEST_FRAME_HEIGHT] __attribute__ ((aligned (16)));


int main( void )
{
//    void minMaxKernel_asm(u8** in, u32 width, u32 height, u8* minVal, u8* maxVal, u8* maskAddr);
	/*width = 24;
	height = 1;
	for(int i = 0 ; i < width; i++)
		input[0][i] = 15;
	
	for(int j = 0 ; j < height; j++)
		for(int i = 0 ; i < width; i++)
			mask[j*width + i] = 1;
	
	mask[5] = 0;
	
	input[0][4] = 20;
	input[0][5] = 1;
	input[0][17] = 21;
	input[0][21] = 1;
	*/
	line = input[0];

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMinMaxPos(&line, width, (u8*)&minVal, (u8*)&maxVal, (u32*)&minPos, (u32*)&maxPos, mask);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvMinMaxPos_opt(&line, width, (u8*)&minVal, (u8*)&maxVal, (u32*)&minPos, (u32*)&maxPos, mask);
		#else
		mvcvMinMaxPos_asm(&line, width, (u8*)&minVal, (u8*)&maxVal, (u32*)&minPos, (u32*)&maxPos, mask);
		#endif
	#endif

	SHAVE_HALT; 
    return 0;
}
