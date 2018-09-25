///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MatrixInv3x3_fp32 kernel function call for unitary test
///

#include "matrixInv3x3_fp32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define SIZE 3

fp32 input[SIZE][SIZE]  __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data")))  output[SIZE][SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

fp32 *in[SIZE];
fp32 *out[SIZE];
fp32 **inMatrix;
fp32 **outMatrix;

int main( void )
{  

  for(int i = 0; i < SIZE; i++)
  {
    in[i] = (fp32*)input[i];
    out[i] = (fp32*)output[i];
  }

  inMatrix = (fp32**)in;
  outMatrix = (fp32**)out;
  
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMatrixInv3x3_fp32(inMatrix, outMatrix);
	#else
	mvcvMatrixInv3x3_fp32_asm(inMatrix, outMatrix);
	#endif
 
	SHAVE_HALT; 
  return 0;
}
