///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     matVecMul kernel function call for unitary test
///

#include "matInv.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>
    
#define TEST_MATRIX_HEIGHT 64
#define TEST_MATRIX_WIDTH 64

//#define UNIT_TEST_USE_C_VERSION

float input1[TEST_MATRIX_HEIGHT][TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output[TEST_MATRIX_HEIGHT][TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

int width __attribute__ ((aligned (16)));
int height __attribute__ ((aligned (16)));

MAT *A;
MAT *out;

float matA[30]; ;//28 is size of mat structure
float matOut[30]; ;//28 is size of mat structure

int main(void)
{
    A   = (MAT *)(&matA[0]); 
    out = (MAT *)(&matOut[0]); 

    A->m    = width;                    out->m    = width;
    A->n    = height;                   out->n    = height;
    A->me   = NULL;                     out->me   = NULL;
    A->base = (float*)input1;           out->base = (float*)output;

   #ifdef UNIT_TEST_USE_C_VERSION
       out = mvcvMatInverseLU(A, out);
   #else
       out=  mvcvMatInverseLU_asm(A, out);
   #endif
    SHAVE_HALT;
    return 0;
}
