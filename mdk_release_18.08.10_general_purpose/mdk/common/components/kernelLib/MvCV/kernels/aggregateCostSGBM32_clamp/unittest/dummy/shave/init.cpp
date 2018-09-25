///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "aggregateCostSGBM32_clamp.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 1920
#define PADDING 16 
#define OFFSET PADDING/2 

u16 __attribute__ ((aligned (16))) penaltyP1;
u16 __attribute__ ((aligned (16))) penaltyP2;
u8 __attribute__ ((aligned (16))) input1[TEST_LINE_WIDTH] ;
u8 __attribute__ ((aligned (16))) input2[TEST_LINE_WIDTH] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marke*/
u8 __attribute__((section(".kept.data"))) output[TEST_LINE_WIDTH + PADDING] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u8 __attribute__ ((aligned (16))) min2;
u8 __attribute__ ((aligned (16))) activateClamp;
u8 __attribute__ ((aligned (16))) *out;

int main( void )
{

	out = (u8 *)output + OFFSET;
	//printf("%x %x \n", in1,out);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvAggregateCostSGBM32_clamp(input1, input2, out, penaltyP1, penaltyP2, min2, activateClamp);
	#else
		mvcvAggregateCostSGBM32_clamp_asm(input1, input2, out, penaltyP1, penaltyP2, min2, activateClamp);
	//mvcvAggregateCostSGBM32_clamp(input1, input2, out, penaltyP1, penaltyP2, min2, activateClamp);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
