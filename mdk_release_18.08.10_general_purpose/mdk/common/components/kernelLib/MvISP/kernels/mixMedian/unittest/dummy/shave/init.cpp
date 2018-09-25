///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     fast9 kernel function call for unitary test
///

#include <mixMedian.h>
#include <stdio.h> 
#include "svuCommonShave.h"
#include <mv_types.h>

#define TEST_FRAME_WIDTH (1920)
#define TEST_FRAME_HEIGHT 1
#define NR_PLANE 3

u8  in0[NR_PLANE][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8  in1[NR_PLANE][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8  reff[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[NR_PLANE][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width __attribute__ ((aligned (16)));
float offset __attribute__ ((aligned (16)));
float slope __attribute__ ((aligned (16)));

u8 *in0L[NR_PLANE];
u8 *in1L[NR_PLANE];
u8 *outL[NR_PLANE];

int main( void )
{
	in0L[0] = &in0[0][0];
	in0L[1] = &in0[1][0];
	in0L[2] = &in0[2][0];

	in1L[0] = &in1[0][0];
	in1L[1] = &in1[1][0];
	in1L[2] = &in1[2][0];
	
	outL[0] = &output[0][0];
	outL[1] = &output[1][0];
	outL[2] = &output[2][0];
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispMixMedian(outL, in0L, in1L, reff, offset, slope, width);
	#else
	mvispMixMedian_asm(outL, in0L, in1L, reff, offset, slope, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
