///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    arithmeticAdd kernel function call for unitary test
///

#include <localTM.h>
#include <mv_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 5

half  input1[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u8    input2[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

half __attribute__ ((aligned (16))) cMat[160] ;

u32 width  __attribute__ ((aligned (16)));
u32 run_no  __attribute__ ((aligned (16)));
half* inLine1[1];
u8* inLine2[2];
half* outLine[1];

half** inLines1;
u8** inLines2;
half** outLines;

half *curves;

int main( void )
{
	inLine1[0] = (half *)input1[0];
	inLine2[0] = (u8*)input2[0] + 8;
	inLine2[1] = (u8*)input2[1] + 8;
	
	outLine[0] = (half*)output[0];
	
	curves = (half *)cMat;
	inLines1 = (half**)inLine1;
	inLines2 = (u8**)inLine2;
	outLines = (half**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispLocalTM(inLines1, inLines2, outLines, curves, width, run_no);
	#else
	mvispLocalTM_asm(inLines1, inLines2, outLines, curves, width, run_no);
	#endif

	SHAVE_HALT;
	return 0;
}
