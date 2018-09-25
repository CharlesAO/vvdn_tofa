///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Convolution7x7 kernel function call for unitary test
///

#include "convolution7x7Fp16ToU8.h"
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
u8 __attribute__((section(".kept.data"))) output[1][TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width = 32;
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
u8* outLine[1];
half** inLines;
u8** outLines;

/*
unsigned char  input[7][48] = {
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}
};
*/



int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (half*)input[i] + 8;
	}
	outLine[0] = (u8*)output[0];

	inLines = (half**)inLine;
	outLines = (u8**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvConvolution7x7Fp16ToU8(inLines, outLines, conv, width);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvConvolution7x7Fp16ToU8_opt(inLines, outLines, conv, width);
		#else
		mvcvConvolution7x7Fp16ToU8_asm(inLines, outLines, conv, width);
		#endif
	#endif
	
	SHAVE_HALT; 
    return 0;
}
