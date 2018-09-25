///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     sgemm4x4 kernel function call for unitary test
///

#include "sgemm4x4.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define MAX_K 2048

float A[MAX_K * 4];
float B[MAX_K * 4];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) C_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) C[    4 * 4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) C_u32postPad[4] __attribute__ ((aligned (16)));

float alpha __attribute__ ((aligned (16)));
float beta __attribute__ ((aligned (16)));
s32   k __attribute__ ((aligned (16)));
s32   rs_c __attribute__ ((aligned (16)));
s32   cs_c __attribute__ ((aligned (16)));

int main( void )
{
#ifdef UNIT_TEST_USE_C_VERSION
    mvcvsgemm4x4(k, &alpha, A, B, &beta, C, rs_c, cs_c);
#else
    mvcvsgemm4x4_asm(k, &alpha, A, B, &beta, C, rs_c, cs_c);
#endif

    SHAVE_HALT;
    return 0;
}
