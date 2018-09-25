///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    thresholdFilter kernel function call for unitary test
///

#include <thresholdFilter.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 2020

fp32 cornerScores[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
fp32 threshold __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));
u32 posOffset __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) candPos_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) candPos[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) candPos_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) candCount_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) candCount[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) candCount_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{
//	void mvcvThresholdFilter(fp32* cornerScores, fp32 threshold, u32 width,
//                            u32 posOffset, u16* candPos, u32* candCount)


	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos, (u32*)candCount);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED	
		mvcvThresholdFilter_opt(cornerScores, threshold, width, posOffset, candPos, (u32*)candCount);
        	#else
        	mvcvThresholdFilter_asm(cornerScores, threshold, width, posOffset, candPos, (u32*)candCount);
		#endif
	#endif
	
	SHAVE_HALT; 
	return 0;
}
