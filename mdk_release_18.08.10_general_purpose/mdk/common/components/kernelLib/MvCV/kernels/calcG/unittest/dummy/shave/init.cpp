///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    calcG kernel function call for unitary test
///

#include <calcG.h>
#include <mv_types.h>
#include <svuCommonShave.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 1920

fp32 ix[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));
fp32 iy[MAX_FRAME_SIZE] __attribute__ ((aligned (16)));

u32 isz_u32[2] __attribute__ ((aligned (16)));
u32 jsz_u32[2] __attribute__ ((aligned (16)));
u32 minI_u32[2] __attribute__ ((aligned (16)));
u32 minJ_u32[2] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32  __attribute__((section(".kept.data"))) g_u32[4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


ClSizeW isz;
ClSizeW jsz;
mvCvPointW minI;
mvCvPointW minJ;
mvScalar g;



int main( void )
{
//	void mvcvCalcG(const fp32* Ix, ClSize isz, mvCvPoint minI, const fp32* Iy, ClSize jsz, mvCvPoint minJ, mvScalar& G);

	isz.x = isz_u32[0];
	isz.y = isz_u32[1];
	jsz.x = jsz_u32[0];
	jsz.y = jsz_u32[1];

	minI.x = minI_u32[0];
	minI.y = minI_u32[1];
	minJ.x = minJ_u32[0];
	minJ.y = minJ_u32[1];

	g[0] = g_u32[0];
	g[1] = g_u32[1];
	g[2] = g_u32[2];
	g[3] = g_u32[3];


	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvCalcG(&ix[500], isz, minI, &iy[500], jsz, minJ, g);
	#else
		mvcvCalcG_asm(&ix[500], isz, minI, &iy[500], jsz, minJ, g);
	#endif


	g_u32[0] = g[0];
	g_u32[1] = g[1];
	g_u32[2] = g[2];
	g_u32[3] = g[3];

	SHAVE_HALT;
    return 0;
}
