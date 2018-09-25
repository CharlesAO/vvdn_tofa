///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     nonMax3x3_fp32 kernel function call for unitary test
///

#include "nonMax3x3_fp32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920

float corners1[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
float corners2[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
float corners3[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
unsigned int candCount __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) candOutCount_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned int __attribute__((section(".kept.data"))) candOutCount[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) candOutCount_u32postPad[4] __attribute__ ((aligned (16)));

unsigned short candPos[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) candOutPos_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned short __attribute__((section(".kept.data"))) candOutPos[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) candOutPos_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{		
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2, corners3, candOutPos, candOutCount);
	#else
	mvcvNonMax3x3_fp32_asm(candPos, candCount, corners1, corners2, corners3, candOutPos, candOutCount);
	#endif	
	
	SHAVE_HALT; 
    return 0;
}
