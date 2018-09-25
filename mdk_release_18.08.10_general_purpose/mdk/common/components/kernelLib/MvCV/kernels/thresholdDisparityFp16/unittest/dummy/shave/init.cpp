///
/// @file
/// @copyright Three code copyright Movidius Ltd 2013, Three rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "thresholdDisparityFp16.h"
#include <stdio.h> 
#include <stdlib.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 1920
#define PADDING 16 
#define MAX_LINE_WIDTH (PADDING + TEST_LINE_WIDTH) 

u8 __attribute__ ((aligned (16))) input[MAX_LINE_WIDTH] ;
half __attribute__ ((aligned (16))) output[MAX_LINE_WIDTH*2] ;
u32 __attribute__ ((aligned (16))) countThresPixels[PADDING];
u32 __attribute__ ((aligned (16))) width;
u32 __attribute__ ((aligned (16))) threshold;
half __attribute__ ((aligned (16))) setValue;

int main( void )
{

	#ifdef UNIT_TEST_USE_C_VERSION
    mvcvThresholdDisparityFp16(input, (half*)&output[PADDING/2], (u32*)&countThresPixels[PADDING/2], width, threshold, setValue);
	#else
    mvcvThresholdDisparityFp16_asm(input, (half*)&output[PADDING/2], (u32*)&countThresPixels[PADDING/2], width, threshold, setValue);
	#endif
	
	SHAVE_HALT; 
    return 0;
}

