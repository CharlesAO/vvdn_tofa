#ifndef __SLAPLACIAN5X5_H__
#define __SLAPLACIAN5X5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter applies a Laplacian filter with 5x5 size  in this case
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian5x5, u8** in, u8** out, u32 inWidth)
//!@}

#endif //__SLAPLACIAN5X5_H__