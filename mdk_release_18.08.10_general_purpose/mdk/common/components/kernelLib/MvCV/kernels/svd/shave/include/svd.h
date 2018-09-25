#ifndef __SINGULAR_VALUE_DECOMPOSITION__
#define __SINGULAR_VALUE_DECOMPOSITION__

#include <mv_types.h>
#include <mvcv_macro.h>

//         SINGULAR VALUE DECOMPOSITION kernel test
//Asm function prototype:
//     void mvcvSVD_asm(float **A, float *S, float **V, int rank);
//
//Asm test function prototype:
//     void svd_asm_test(float **A, float *S, float **V, int rank);
//
//C function prototype:
//     void mvcvSVD(float **A, float *S, float **V, int rank);
//
//Parameter description:
//!@{
/// mvcvSVD 	- Performs a singular value decomposition A = USV' of square matrix.
///               No restrictions to size or contents of the matrix.
///               This version of SVD works on square matrices only.
///               The assembly code for size 3 and size 9 is optimized particularly.
///
/// @param[in]   A  rank x rank matrix - on calling contains the matrix to be decomposed	
/// @param[out]  A  rank x rank matrix - on return contains the product US	
/// @param[out]  S  vector returns the square of the singular values. 
/// @param[out]  V  rank x rank matrix - on return contains V(not V')   	
/// @param[in]   rank -number of rows/columns in matrix A           	 
/// @return      Nothing


MVCV_FUNC(void, mvcvSVD, float **A, float *S, float **V, int rank)
//!@}
	
#endif //__SINGULAR_VALUE_DECOMPOSITION__