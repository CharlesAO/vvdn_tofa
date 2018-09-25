///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    whiteBalanceBayerGBGR kernel function call for unitary test
///

#include "whiteBalanceBayerGBRG.h"
#include "mv_types.h"
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 2

u16 input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]   __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) output[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

u16* inLine[TEST_FRAME_HEIGHT];
u16* outLine[TEST_FRAME_HEIGHT];
 
u16** inLines;
u16** outLines;

u16 clamp[1]   __attribute__ ((aligned (16)));
u16 awbCoef[3] __attribute__ ((aligned (16)));
u8 line 	   __attribute__ ((aligned (16)));
u32 width __attribute__ ((aligned (16)));

int main( void )
{
//	void whiteBalanceBayerGBRG(u16** inGBRG, u16 awbCoef[3], u16** outGBRG, u16 clamp[1], u32 width, u8 line)

	inLine[0] = (u16*)input[0];
	outLine[0] = (u16*)output[0];
	
	inLines = (u16**)inLine;
	outLines = (u16**)outLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvWhiteBalanceBayerGBRG(inLines, awbCoef, outLines, clamp, width, line) ;
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvWhiteBalanceBayerGBRG_opt(inLines, awbCoef, outLines, clamp, width, line) ;
		#else 
		mvcvWhiteBalanceBayerGBRG_asm(inLines, awbCoef, outLines, clamp, width, line) ;
		#endif

	#endif
	
	SHAVE_HALT; 
    return 0;
}
