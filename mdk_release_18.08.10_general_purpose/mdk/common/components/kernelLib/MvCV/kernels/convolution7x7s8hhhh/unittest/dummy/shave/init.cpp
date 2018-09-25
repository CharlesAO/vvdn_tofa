///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution7x7 kernel function call for unitary test
///

#include "convolution7x7s8hhhh.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 7

half input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width = 40;
half conv[49] = {
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, 
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, 
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, 
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, 
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, 
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02,
 (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02
 };
half* inLine[TEST_FRAME_HEIGHT];
half* outLine[1];
half** inLines;
half** outLines;


//half  input[7][56] = {
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}, 
//	{(half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02, (half)0.02}
//};


int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i] + 8;
	}
	outLine[0] = (half*)output[0];

	inLines = (half**)inLine;
	outLines = (half**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvolution7x7s8hhhh(inLines, outLines, conv, width);
	#else
	mvcvConvolution7x7s8hhhh_asm(inLines, outLines, conv, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
