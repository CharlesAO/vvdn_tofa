///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     strsm_l4x4 kernel function call for unitary test
///

#include "strsm_l4x4.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define ALIGN(_value) __attribute__((aligned(_value)))
#define MAX_K 2048

float ALIGN(16) a[4 * 4];
float ALIGN(16) b[4 * 4];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) c_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) c[MAX_K] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) c_u32postPad[4] __attribute__ ((aligned (16)));

int   rs_c __attribute__ ((aligned (16)));
int   cs_c __attribute__ ((aligned (16)));

__attribute__((dllexport)) int main(void)
{

#ifdef UNIT_TEST_USE_C_VERSION
    lamaStrsm_l4x4(a, b, c_Asm, rs_c, cs_c);
#else
    lamaStrsm_l4x4_asm(a, b, c, rs_c, cs_c);
#endif


    SHAVE_HALT;
    return 0;
}
