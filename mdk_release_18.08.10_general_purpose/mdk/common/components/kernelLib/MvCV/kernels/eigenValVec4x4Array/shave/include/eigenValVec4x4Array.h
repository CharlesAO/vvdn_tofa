#ifndef __mvcvEigenValVec4x4Array_H__
#define __mvcvEigenValVec4x4Array_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// mvcvEigenValVec4x4Array -  kernel that computes for a number of 4x4 matrices minimum eigenvals and the eigen vector coresponding to iter_swap
/// @param[in] matrix          - pointer for 4x4 matrices to be computed
/// @param[in] threshold       - threshold to determine number of loops for each matrix computation 
/// @param[in] num_matrix      - number of matrices wich for minimum eigenvals and eigenvector will be computed
/// @param[out] eigvals        - pointer for computed minimum eigenvalues of matrices
/// @param[out] eigvec         - pointer for eigenvectors computed


MVCV_FUNC(void, mvcvEigenValVec4x4Array, float** matrix, float* eigvals, float** eigvec, u32 num_matrix, float threshold)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvEigenValVec4x4Array_opt( float** matrix, float* eigvals, float** eigvec, u32 num_matrix, float threshold);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__mvcvEigenValVec4x4Array_H__
