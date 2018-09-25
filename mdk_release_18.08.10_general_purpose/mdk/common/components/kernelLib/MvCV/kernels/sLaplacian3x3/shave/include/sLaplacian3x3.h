#ifndef __SLAPLACIAN3X3_H__
#define __SLAPLACIAN3X3_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter applies a Laplacian filter with 3x3 size in this case
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian3x3, u8** in, u8** out, u32 inWidth)
//!@}

#endif //__SLAPLACIAN3X3_H__