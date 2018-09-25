#ifndef __SLAPLACIAN7X7_H__
#define __SLAPLACIAN7X7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter applies a Laplacian filter with 7x7 size in this case 
/// ( see http://en.wikipedia.org/wiki/Discrete_Laplace_operator ) 
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSLaplacian7x7, u8** in, u8** out, u32 inWidth)
//!@}

#endif //__SLAPLACIAN7X7_H__