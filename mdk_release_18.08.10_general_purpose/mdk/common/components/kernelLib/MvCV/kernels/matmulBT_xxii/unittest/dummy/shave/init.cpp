///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///
    
#include "matmulBT_xxii.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH (100)


unsigned char __attribute__ ((aligned (16))) matA[TEST_FRAME_WIDTH * TEST_FRAME_WIDTH] ;
unsigned char __attribute__ ((aligned (16))) matB[TEST_FRAME_WIDTH * TEST_FRAME_WIDTH] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) matC_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned int __attribute__((section(".kept.data"))) matC[TEST_FRAME_WIDTH * TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) matC_u32postPad[4] __attribute__ ((aligned (16)));


unsigned int m;
unsigned int k; 
unsigned int n;
unsigned int wA;
unsigned int wB;
unsigned int wC;

int main( void )
{
       
    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvMatmulBT_xxii(matA, matB, matC, m, k, n, wA, wB, wC);
    #else
        mvcvMatmulBT_xxii_asm(matA, matB, matC, m, k, n, wA, wB, wC);
    #endif
    
    SHAVE_HALT; 
    return 0;
}
