///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    cvtColorRGBtoUV kernel function call for unitary test
///

#include "cvtColorRGBtoChromaNV12.h"
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH 1920
#define TEST_FRAME_HEIGHT 3

u8 input1RGB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));;
u8 input2RGB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));;
//u8 inputB[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH]  __attribute__ ((aligned (16)));;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outputUV_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) outputUV[TEST_FRAME_WIDTH + 32] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outputUV_u32postPad[4] __attribute__ ((aligned (16)));


u32 width;
float mat[3*3];
float offset[1*3];


u8* inLine1RGB[TEST_FRAME_HEIGHT];
u8* inLine2RGB[TEST_FRAME_HEIGHT];

//u8* inLineB[TEST_FRAME_HEIGHT];

u8** inLines1RGB;
u8** inLines2RGB;

//u8** inLinesB;


int main( void )
{
//	
	int i;
	for (i = 0; i < TEST_FRAME_HEIGHT; i++) {
		inLine1RGB[i] = (u8*)input1RGB[i];
		inLine2RGB[i] = (u8*)input2RGB[i];
	//	inLineB[i] = (u8*)inputB[i];
	}
	
	inLines1RGB = (u8**)inLine1RGB;
	inLines2RGB = (u8**)inLine2RGB;
	//inLinesB = (u8**)inLineB;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorRGBtoChromaNV12(inLines1RGB, inLines2RGB, outputUV, mat, offset, width);
	#else
	mvcvCvtColorRGBtoChromaNV12_asm(inLines1RGB, inLines2RGB, outputUV, mat, offset, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
