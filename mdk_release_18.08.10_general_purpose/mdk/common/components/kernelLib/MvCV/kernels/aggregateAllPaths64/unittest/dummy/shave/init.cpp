///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "aggregateAllPaths64.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 320
#define PADDING 16
#define OFFSET PADDING/2



u8 __attribute__ ((aligned (16))) path0[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path1[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path2[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path3[TEST_LINE_WIDTH * DISPARITIES] ;


/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_LINE_WIDTH * DISPARITIES + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_postPad[4] __attribute__ ((aligned (16)));

u8 __attribute__ ((aligned (16))) *ptrOutput;
u32 __attribute__ ((aligned (16))) width;

int main( void )
{

	ptrOutput = (u8 *)output + OFFSET;
	//printf("%x %x \n", in1,out);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvAggregateAllPaths64(path0, path1, path2, path3, ptrOutput, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvAggregateAllPaths64_opt(path0, path1, path2, path3, ptrOutput, width);
	#else
		mvcvAggregateAllPaths64_asm(path0, path1, path2, path3, ptrOutput, width);
	#endif
	#endif
	SHAVE_HALT;
    return 0;
}
