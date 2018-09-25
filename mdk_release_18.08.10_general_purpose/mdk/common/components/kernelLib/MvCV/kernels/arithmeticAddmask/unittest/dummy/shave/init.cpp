///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    arithmeticAdd kernel function call for unitary test
///

#include "arithmeticAddmask.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920 + 8
#define TEST_FRAME_HEIGHT 1

u8 input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 mask[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width;
u8* inLine1[TEST_FRAME_HEIGHT];
u8* inLine2[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];
u8* mmask[TEST_FRAME_HEIGHT];
u8** inLines1;
u8** inLines2;
u8** outLines;
u8** mmaskk;


int main( void )
{
//	void arithmeticAddmask(u8** in1, u8** in2, u8** out, u8** mask, u32 width)


	int i;
	for(i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine1[i] = (u8*)input1[i];
		inLine2[i] = (u8*)input2[i];
		outLine[i] = (u8*)output[i];
		mmask[i] = (u8*)mask[i];
	}

	inLines1 = (u8**)inLine1;
	inLines2 = (u8**)inLine2;
	outLines = (u8**)outLine;
	mmaskk = (u8**)mmask;
		    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvArithmeticAddmask(inLines1, inLines2, outLines, mmaskk, width);
	#else
	mvcvArithmeticAddmask_asm(inLines1, inLines2, outLines, mmaskk, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
