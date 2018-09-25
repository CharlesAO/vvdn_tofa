///
/// @file
/// @copyright Three code copyright Movidius Ltd 2013, Three rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "aggregateFivePaths32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 320
#define DISPARITIES 32 
#define PADDING 16 
#define OFFSET PADDING/2 



u8 __attribute__ ((aligned (16))) path0[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path1[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path2[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path3[TEST_LINE_WIDTH * DISPARITIES] ;
u8 __attribute__ ((aligned (16))) path4[TEST_LINE_WIDTH * DISPARITIES] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) out_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) out[TEST_LINE_WIDTH * DISPARITIES + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_u32postPad[4] __attribute__ ((aligned (16)));

u8 __attribute__ ((aligned (16))) *output;
u32 __attribute__ ((aligned (16))) width;

int main( void )
{

	output = (u8 *)out + OFFSET;
	//printf("%x %x \n", in1,out);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvAggregateFivePaths32(path0, path1, path2, path3, path4, output, width);
	#else
		mvcvAggregateFivePaths32_asm(path0, path1, path2, path3, path4, output, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
