///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

#include <meanStdDev.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) mean_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) mean __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) mean_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) stdev_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) stdev __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) stdev_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));
u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8** inLines;
u8* inLine[TEST_FRAME_HEIGHT];



int main( void )
{

	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
	}
	inLines = (u8**)inLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMeanstddev(inLines, &mean, &stdev, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvMeanstddev_opt(inLines, &mean, &stdev, width);
		#else
		mvcvMeanstddev_asm(inLines, &mean, &stdev, width);
		#endif
	#endif

//	*mean=5;
//	*stdev=6;
	SHAVE_HALT; 
    return 0;
}
