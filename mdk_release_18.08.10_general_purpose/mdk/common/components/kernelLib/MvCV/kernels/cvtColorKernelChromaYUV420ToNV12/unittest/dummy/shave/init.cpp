///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     cvtColor kernel function call for unitary test
///

#include "cvtColorKernelChromaYUV420ToNV12.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

//#define UNIT_TEST_USE_C_VERSION

#define MAX_WIDTH 1920

u8 __attribute__ ((aligned (32))) inU[MAX_WIDTH/2];
u8 __attribute__ ((aligned (32))) inV[MAX_WIDTH/2];
u32 __attribute__ ((aligned (32))) width;

uint32_t  __attribute__((section(".kept.data"))) __attribute__ ((aligned (32))) output_u32prePad[4];
u8 __attribute__ ((section(".kept.data"))) __attribute__ ((aligned (32))) outUV[MAX_WIDTH] ;
uint32_t __attribute__((section(".kept.data"))) __attribute__ ((aligned (32))) output_u32postPad[4];

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvCvtColorKernelChromaYUV420ToNV12(inU, inV, outUV, width);
	#else
	mvcvCvtColorKernelChromaYUV420ToNV12_asm(inU, inV, outUV, width);
	#endif

	SHAVE_HALT; 
    return 0;
}
