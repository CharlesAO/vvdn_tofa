///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     ///matrix set block


#include "matSet.h"
#include <stdio.h>
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define TEST_MATRIX_HEIGHT 64
#define TEST_MATRIX_WIDTH 64
#define EDGE 16

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) inpu1_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
float __attribute__((section(".kept.data"))) input1[TEST_MATRIX_HEIGHT * TEST_MATRIX_WIDTH + EDGE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) input1_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width __attribute__ ((aligned (16)));
unsigned int height __attribute__ ((aligned (16)));

MAT *A;
MAT *C;


float matA[30]; ;//28 is size of mat structure
float matC[30]; ;//28 is size of mat structure

float scalar __attribute__ ((aligned (16)));
int start_row __attribute__ ((aligned (16)));
int start_col __attribute__ ((aligned (16)));
int length_row __attribute__ ((aligned (16)));
int length_col __attribute__ ((aligned (16)));

int main(void)
{
	A   = (MAT *)(&matA[0]); 
	C   = (MAT *)(&matC[0]); 
	



	A->m    = height;        
	A->n    = width;         
	A->base = (float*)input1;
	


#ifdef UNIT_TEST_USE_C_VERSION
    C = mvcvMatSet(scalar, A, start_row, start_col, length_row, length_col);
#else
    C = mvcvMatSet_asm(scalar, A, start_row, start_col, length_row, length_col);
#endif

    SHAVE_HALT;
    return 0;
}
