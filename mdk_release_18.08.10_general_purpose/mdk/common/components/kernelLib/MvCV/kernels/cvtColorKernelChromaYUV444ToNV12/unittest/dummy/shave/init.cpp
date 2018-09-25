///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     cvtColor kernel function call for unitary test
///

#include "cvtColorKernelChromaYUV444ToNV12.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

//#define UNIT_TEST_USE_C_VERSION

#define MAX_WIDTH 1920

u8 __attribute__ ((aligned (32))) inU[2][MAX_WIDTH];
u8 __attribute__ ((aligned (32))) inV[2][MAX_WIDTH];
u32 __attribute__ ((aligned (32))) width;

uint32_t  __attribute__((section(".kept.data"))) __attribute__ ((aligned (32))) output_u32prePad[4];
u8 __attribute__ ((section(".kept.data"))) __attribute__ ((aligned (32))) outUV[MAX_WIDTH] ;
uint32_t __attribute__((section(".kept.data"))) __attribute__ ((aligned (32))) output_u32postPad[4];

int main( void )
{
	u8 *inULines[2], *inVLines[2];

	inULines[0]=inU[0]; inULines[1]=inU[1];
	inVLines[0]=inV[0]; inVLines[1]=inV[1];

	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorKernelChromaYUV444ToNV12(inULines, inVLines, outUV, width);
	#else
	mvcvCvtColorKernelChromaYUV444ToNV12_asm(inULines, inVLines, outUV, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
