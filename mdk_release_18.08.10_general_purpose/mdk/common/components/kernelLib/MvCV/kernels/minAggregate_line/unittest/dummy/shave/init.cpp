///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "minAggregate_line.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 640
#define DISPARITIES 64
#define PADDING 16 
#define OFFSET PADDING/2 

u32 __attribute__ ((aligned (16))) penaltyP1[TEST_LINE_WIDTH];
u32 __attribute__ ((aligned (16))) penaltyP2[TEST_LINE_WIDTH];
u8 __attribute__ ((aligned (16))) input1[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) input2[TEST_LINE_WIDTH * DISPARITIES] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[(TEST_LINE_WIDTH*DISPARITIES) + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 __attribute__ ((aligned (16))) width;
u8  __attribute__ ((aligned (16))) direction;
u8  __attribute__ ((aligned (16))) activateClamp;
u8  __attribute__ ((aligned (16))) *out;
u32 __attribute__ ((aligned (16))) disparities;
int main( void )
{
	out = (u8 *)output + OFFSET;

	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvMinAggregate_line(input1, input2, out, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	#else
		mvcvMinAggregate_line_asm(input1, input2, out, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
