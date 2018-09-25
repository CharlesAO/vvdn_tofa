	///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Histogram kernel function call for unitary test
///

#include <histogram.h>
#include <stdio.h>   
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

u32 width;
u32 hist[256];
u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;

__attribute__((dllexport)) int main( void )
{
    for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i];
	}
    
    inLines = (u8**)inLine;
     
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvHistogram(inLines, hist, width);
	#else
	mvcvHistogram_asm(inLines, hist, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
