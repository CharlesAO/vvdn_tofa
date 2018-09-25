///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    whiteBalanceRGB kernel function call for unitary test
///

#include <whiteBalanceRGB.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u16 inputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]__attribute__ ((aligned (16)));
u16 inputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u16 inputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputR_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) outputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputR_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputG_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) outputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputG_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputB_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) outputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputB_u32postPad[4] __attribute__ ((aligned (16)));

u16* inLineR[TEST_FRAME_HEIGHT];
u16* inLineG[TEST_FRAME_HEIGHT];
u16* inLineB[TEST_FRAME_HEIGHT];
u16* outLineR[TEST_FRAME_HEIGHT];
u16* outLineG[TEST_FRAME_HEIGHT];
u16* outLineB[TEST_FRAME_HEIGHT];
 
u16** inLinesR;
u16** inLinesG;
u16** inLinesB;
u16** outLinesR;
u16** outLinesG;
u16** outLinesB;

u16 clamp[1] __attribute__ ((aligned (16)));
u16 awbCoef[3] __attribute__ ((aligned (16)));

u32 width __attribute__ ((aligned (16)));

int main( void )
{
//	 void whiteBalanceRGB(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], 16** oR, u16** oG, u16** oB, u16 clamp[1], u32 width) 

	inLineR[0] = (u16*)inputR[0];
	inLineG[0] = (u16*)inputG[0];
	inLineB[0] = (u16*)inputB[0];
	outLineR[0] = (u16*)outputR[0];
	outLineG[0] = (u16*)outputG[0];
	outLineB[0] = (u16*)outputB[0];
	
	inLinesR = (u16**)inLineR;
	inLinesG = (u16**)inLineG;
	inLinesB = (u16**)inLineB;
	outLinesR = (u16**)outLineR;
	outLinesG = (u16**)outLineG;
	outLinesB = (u16**)outLineB;

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvWhiteBalanceRGB(inLinesR, inLinesG, inLinesB, awbCoef, outLinesR, outLinesG, outLinesB, clamp, width);
	#else
        #ifdef MOVICOMPILE_OPTIMIZED
		mvcvWhiteBalanceRGB_opt(inLinesR, inLinesG, inLinesB, awbCoef, outLinesR, outLinesG, outLinesB, clamp, width);
		#else
		mvcvWhiteBalanceRGB_asm(inLinesR, inLinesG, inLinesB, awbCoef, outLinesR, outLinesG, outLinesB, clamp, width);
		#endif
	#endif
	
	SHAVE_HALT; 
    return 0;
}
