///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     matVecMul kernel function call for unitary test
///

#include "matVecMul.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define MAX_K 128

float A[MAX_K]  __attribute__ ((aligned (16)));
float B[MAX_K * MAX_K]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) C_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) C[MAX_K] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) C_u32postPad[4] __attribute__ ((aligned (16)));

s32 k  __attribute__ ((aligned (16)));
s32 n  __attribute__ ((aligned (16)));

int main(void)
{

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvMatVecMul(A, B, C, k, n);
#else
    mvcvMatVecMul_asm(A, B, C, k, n);
#endif

    SHAVE_HALT;
    return 0;
}
