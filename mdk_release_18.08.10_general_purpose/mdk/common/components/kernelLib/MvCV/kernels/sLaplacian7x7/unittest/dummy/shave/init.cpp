///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoNV21 kernel function call for unitary test
///

#include <sLaplacian7x7.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920+8
#define TEST_FRAME_HEIGHT 7

u8 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16))); 

u8* inLine[TEST_FRAME_HEIGHT];
u8* outLine[TEST_FRAME_HEIGHT];

u8** inLines;
u8** outLines;



int main( void )
{
//	void sLaplacian7x7_asm(u8** in, u8** out, u32 inWidth);

	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)input[i] + 4;
	
	}
	outLine[0] = (u8*)output;
	
	inLines = (u8**)inLine;
	outLines = (u8**)outLine;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvSLaplacian7x7(inLines, outLines, width);
	#else
	mvcvSLaplacian7x7_asm(inLines, outLines, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
