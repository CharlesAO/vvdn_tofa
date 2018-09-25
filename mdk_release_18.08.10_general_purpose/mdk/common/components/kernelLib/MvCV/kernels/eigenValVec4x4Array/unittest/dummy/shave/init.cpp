#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <eigenValVec4x4Array.h>
#include <moviVectorTypes.h>
#include "svuCommonShave.h"


#define num_mat 1000
#define mat_dim 4


	float input[num_mat][mat_dim * mat_dim] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_eigen_vecprePad __attribute__ ((aligned (16)));
/*output data marker*/
	float __attribute__((section(".kept.data"))) out_eigen_vec[num_mat][mat_dim * mat_dim] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_eigen_vecpostPad __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_eigen_valprePad __attribute__ ((aligned (16)));
/*output data marker*/
	float __attribute__((section(".kept.data"))) out_eigen_val[num_mat] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) out_eigen_valpostPad __attribute__ ((aligned (16)));

	float *in[num_mat];
	
	float *vector[num_mat];
	
	float **in_array;
	
	float **eigenvec;
	
	u32 num_matrix ;//= 2;
	float threshold;
int main()
{

	
	for (int i =0; i< num_mat; i++)
	{
		in[i]     = (float*) input[i];
		vector[i] = (float*) out_eigen_vec[i];

	}
	
	in_array = (float**) in;
    
    eigenvec = (float**) vector;
   
	//printf("%x %x %x \n", in_array, out_eigen_val, eigenvec);
    #ifdef UNIT_TEST_USE_C_VERSION
	mvcvEigenValVec4x4Array( in_array, out_eigen_val, eigenvec, num_matrix, threshold);
	#else
		#ifdef MOVICOMPILE_OPTIMIZED
		mvcvEigenValVec4x4Array_opt( in_array, out_eigen_val, eigenvec, num_matrix, threshold);
		#else
		mvcvEigenValVec4x4Array_asm( in_array, out_eigen_val, eigenvec, num_matrix, threshold);
		#endif
	#endif
		
	SHAVE_HALT; 
	return 0;
}
