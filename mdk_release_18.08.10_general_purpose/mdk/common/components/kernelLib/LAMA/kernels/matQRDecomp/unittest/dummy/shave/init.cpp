///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     matQRDecomp kernel function call for unitary test
///

#include <matQRDecomp.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define EDGE 16
//#define UNIT_TEST_USE_C_VERSION
float input[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output1_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output1[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH + EDGE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output1_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output2_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) output2[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH + EDGE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output2_u32postPad[4] __attribute__ ((aligned (16)));

float baseq0[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));
float baseq1[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));
float basez0[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));
float basez1[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH] __attribute__ ((aligned (16)));
MAT Q0, Q1, Z0, Z1;
MAT *q0 , *q1 , *z0, *z1;

unsigned int width __attribute__ ((aligned (16)));
unsigned int height __attribute__ ((aligned (16)));

MAT *A;
MAT *Q;
MAT *R;

MAT matA;
MAT matQ;
MAT matR;

void init_temp_matrix(int width, int height)
{
    memset(baseq0,0,sizeof(Real)*height*height);
    memset(baseq1,0,sizeof(Real)*height*height);
    memset(basez0,0,sizeof(Real)*height*width);
    memset(basez1,0,sizeof(Real)*height*width);

    Q0.m = Q1.m = Z0.m = Z1.m = height;
    Q0.n = Q1.n = height;
    Z0.n = Z1.n = width;

    Q0.base = baseq0;
    Q1.base = baseq1;
    Z0.base = basez0;
    Z1.base = basez1;

    q0 = &Q0;
    q1 = &Q1;
    z0 = &Z0;
    z1 = &Z1;
}

int main(void)
{

	init_temp_matrix(width, height);

	A   = &matA;
	Q   = &matQ;
	R   = &matR;

	A->m    = height;         Q->m    = height;            R->m    = height;
	A->n    = width;          Q->n    = height;            R->n    = width;

	A->base = (float*)input;		 Q->base = (float*)output1; 		  R->base = (float*)output2;

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvMatQRDecomp(A, Q, R, q0, q1, z0, z1);
#else
    mvcvMatQRDecomp_asm(A, Q, R, q0, q1, z0, z1);
#endif
    SHAVE_HALT;
    return 0;
}
