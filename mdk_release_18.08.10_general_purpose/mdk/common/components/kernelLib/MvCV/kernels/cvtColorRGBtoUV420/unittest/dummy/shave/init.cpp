///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoUV kernel function call for unitary test
///

#include "cvtColorRGBtoUV420.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 1

u8 inputR[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;
u8 inputG[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;
u8 inputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputU_u32prePad[4] __attribute__ ((aligned (16)))  ;
u8 __attribute__((section(".kept.data")))  outputU[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputU_u32postPad[4] __attribute__ ((aligned (16)))   ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputV_u32prePad[4] __attribute__ ((aligned (16)))  ;
u8 __attribute__((section(".kept.data")))  outputV[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputV_u32postPad[4] __attribute__ ((aligned (16)))   ;

u32 width;

u8* inLineR[TEST_FRAME_HEIGHT];
u8* inLineG[TEST_FRAME_HEIGHT];
u8* inLineB[TEST_FRAME_HEIGHT];
u8* outLineU[TEST_FRAME_HEIGHT];
u8* outLineV[TEST_FRAME_HEIGHT];

u8** inLinesR;
u8** inLinesG;
u8** inLinesB;
u8** outLinesU;
u8** outLinesV;

int main( void )
{
//	void cvtColorRGBtoUV420_asm(u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width) 

	inLineR[0] = (u8*)inputR[0];
	inLineG[0] = (u8*)inputG[0];
	inLineB[0] = (u8*)inputB[0];
	outLineU[0] = (u8*)outputU[0];
	outLineV[0] = (u8*)outputV[0];
	
	inLinesR = (u8**)inLineR;
	inLinesG = (u8**)inLineG;
	inLinesB = (u8**)inLineB;
	outLinesU = (u8**)outLineU;
	outLinesV = (u8**)outLineV;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorRGBtoUV420(inLinesR, inLinesG, inLinesB, outLinesU, outLinesV, width);
	#else
	mvcvCvtColorRGBtoUV420_asm(inLinesR, inLinesG, inLinesB, outLinesU, outLinesV, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
