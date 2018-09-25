///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    thresholdFilter kernel function call for unitary test
///

#include <thresholdFilterS16.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 1920

short line[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
u32 threshold __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) indexList_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) indexList[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) indexList_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) count_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) count __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) count_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{
   
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvThresholdFilterS16(line,threshold, width,indexList, &count);
		
	#else	
		mvcvThresholdFilterS16_asm(line,threshold, width,indexList, &count);
	#endif
	
	
	SHAVE_HALT; 
	return 0;
}
