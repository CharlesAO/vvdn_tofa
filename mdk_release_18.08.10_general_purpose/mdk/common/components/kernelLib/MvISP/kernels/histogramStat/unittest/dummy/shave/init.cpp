	///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     HistogramStat kernel function call for unitary test
///

#include <histogramStat.h>
#include <stdio.h>   
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define HIST_SIZE 64

u32 step __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) histR_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) histR[HIST_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) histR_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) histG_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) histG[HIST_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) histG_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) histB_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u32 __attribute__((section(".kept.data"))) histB[HIST_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) histB_u32postPad[4] __attribute__ ((aligned (16)));

u8 inputR[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inputG[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8 inputB[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));


int main( void )
{
         
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispHistogramStat(inputR, inputG, inputB, histR, histG, histB, width, step);
	#else
	mvispHistogramStat_asm(inputR, inputG, inputB, histR, histG, histB, width, step);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
