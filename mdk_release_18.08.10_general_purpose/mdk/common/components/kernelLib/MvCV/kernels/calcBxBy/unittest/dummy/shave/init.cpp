///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    calcBxBy kernel function call for unitary test
///

#include <calcBxBy.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>


#define MAX_FRAME_SIZE 1920

u8 patchI[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
u8 patchJ[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
fp32 Ix[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
fp32 Iy[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
u32 isz_u32[2] __attribute__ ((aligned (16)));
u32 jsz_u32[2] __attribute__ ((aligned (16)));
u32 minI_u32[2] __attribute__ ((aligned (16)));
u32 minJ_u32[2] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) errorPtr_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32  __attribute__((section(".kept.data"))) errorPtr[1] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) errorPtr_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) retVal_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32  __attribute__((section(".kept.data"))) retVal_fp32[2] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) retVal_u32postPad[4] __attribute__ ((aligned (16)));

ClSizeW isz;
ClSizeW jsz;
mvCvPointW minI;
mvCvPointW minJ;
mvCvPoint2D32f retVal;


int main( void )
{
//	mvCvPoint2D32f mvcvCalcBxBy(const unsigned char* patchI, unsigned int* isz, const float* Ix, const float* Iy, 
//				unsigned int* minI, const unsigned char* patchJ, unsigned int* jsz, unsigned int* minJ, float& error);

	isz.x = isz_u32[0];
	isz.y = isz_u32[1];	
	jsz.x = jsz_u32[0];
	jsz.y = jsz_u32[1];
	
	minI.x = minI_u32[0];
	minI.y = minI_u32[1];
	minJ.x = minJ_u32[0];
	minJ.y = minJ_u32[1];

	
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvCalcBxBy(patchI, isz, Ix, Iy, minI, patchJ, jsz, minJ, errorPtr);
	#else
		retVal = mvcvCalcBxBy_asm(patchI, isz, Ix, Iy, minI, patchJ, jsz, minJ, errorPtr);		
	#endif

	retVal_fp32[0] = retVal.x;
	retVal_fp32[1] = retVal.y;

	SHAVE_HALT; 
	return 0;
}
