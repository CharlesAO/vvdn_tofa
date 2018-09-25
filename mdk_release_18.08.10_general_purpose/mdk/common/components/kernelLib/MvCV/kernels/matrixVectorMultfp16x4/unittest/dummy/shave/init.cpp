///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     matrixVectorMultfp16x4 kernel function call for unitary test
///

#include "matrixVectorMultfp16x4.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define ALIGN(_value) __attribute__((aligned(_value)))
#define TEST_ARRAY_SIZE (10*16)

half ALIGN(16) inputA[4][TEST_ARRAY_SIZE] __attribute__ ((aligned (16)));
half ALIGN(16) inputB[TEST_ARRAY_SIZE] __attribute__ ((aligned (16)));


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) outVector_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
half __attribute__((section(".kept.data"))) outVector[4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) outVector_u32postPad[4] __attribute__ ((aligned (16)));

u32 num_elements __attribute__ ((aligned (16)));

int main( void )
{
    half* inA[4];

    inA[0] = (half*)inputA[0];
    inA[1] = (half*)inputA[1];
    inA[2] = (half*)inputA[2];
    inA[3] = (half*)inputA[3];

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvmatrixVectorMultfp16x4(inA, inputB, num_elements, outVector);
#else
#ifdef MOVICOMPILE_OPTIMIZED
        mvcvmatrixVectorMultfp16x4_opt(inA, inputB, num_elements, outVector);
	#else
	mvcvmatrixVectorMultfp16x4_asm(inA, inputB, num_elements, outVector);
	#endif
#endif

    SHAVE_HALT;
    return 0;
}

