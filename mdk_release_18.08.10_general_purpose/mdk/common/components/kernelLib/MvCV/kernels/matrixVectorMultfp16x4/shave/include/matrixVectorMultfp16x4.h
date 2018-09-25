#ifndef __MATRIXVECTORMULTFP16x4_H__
#define __MATRIXVECTORMULTFP16x4_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// multiply 4xN matrix by Nx1 vector to give 4x1 result vector.
/// N is specified by num_elements and is always be a multiple of 16
/// @param[in]  inArrayA     - Input rows of the 4xN matrix
/// @param[in]  inVecB       - Nx1 vector
/// @param[in]  num_elements - N - number of elements
/// @param[out] outVec       - 4x1 result vector

MVCV_FUNC(void, mvcvmatrixVectorMultfp16x4, half **inArrayA, half *inVecB, u32 num_elements, half *outVec)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvmatrixVectorMultfp16x4_opt(half **inArrayA, half *inVecB, u32 num_elements, half *outVec);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__MATRIXVECTORMULTFP16x4_H__
