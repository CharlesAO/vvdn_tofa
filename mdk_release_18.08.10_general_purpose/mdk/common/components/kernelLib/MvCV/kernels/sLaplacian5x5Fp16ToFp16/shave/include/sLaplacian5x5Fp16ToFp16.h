#ifndef __SLAPLACIAN5x5Fp16ToFp16_H__
#define __SLAPLACIAN5x5Fp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter applies a Laplacian filter with 5x5 size in this case
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines, 16-bits floating point
/// @param[out] out   - Output line, 16-bits floating point
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian5x5Fp16ToFp16, half** in, half** out, u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C"
{
	void mvcvSLaplacian5x5Fp16ToFp16_opt(half** in, half** out, u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__SLAPLACIAN5x5Fp16ToFp16_H__
