///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     sgemmtrsm_l4x4 kernel function call for unitary test
///

#include "sgemmtrsm_l4x4.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define ALIGN(_value) __attribute__((aligned(_value)))
#define MAX_K 2048

float ALIGN(16) A10[MAX_K * 4];
float ALIGN(16) A11[    4 * 4];
float ALIGN(16) B01[MAX_K * 4];
/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) B11_u32prePad[4] __attribute__ ((aligned (16)));
float __attribute__((section(".kept.data"))) B11[    4 * 4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) B11_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) C11_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) C11[4 * 4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) C11_u32postPad[4] __attribute__ ((aligned (16)));

float alpha __attribute__ ((aligned (16)));
s32   k __attribute__ ((aligned (16))); 
s32   rs_c __attribute__ ((aligned (16)));
s32   cs_c __attribute__ ((aligned (16)));

__attribute__((dllexport)) int main( void )
{
#ifdef UNIT_TEST_USE_C_VERSION
    mvcvsgemmtrsm_l4x4(k, &alpha, A10, A11, B01, B11, C11, rs_c, cs_c);
#else
    mvcvsgemmtrsm_l4x4_asm(k, &alpha, A10, A11, B01, B11, C11, rs_c, cs_c);
#endif
    SHAVE_HALT;
    return 0;
}
