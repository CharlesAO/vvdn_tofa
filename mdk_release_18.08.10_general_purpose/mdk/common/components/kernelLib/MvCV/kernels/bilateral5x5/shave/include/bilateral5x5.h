#ifndef __BILATERAL5x5_H__
#define __BILATERAL5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernels performs a bilateral filter on the input image of 5x5 dimensions
///@param[in]	input	- Input lines, 16-bits unsigned char
///@param[in]	sigma	- Sigma values, 16-bits unsigned char
///@param[in] 	width	- Width of input line
///@param[out]	output	- Output lines, 16-bits unsigned char

MVCV_FUNC(void, mvcvBilateral5x5, u16* input, u16* sigma, u32 width, u16* output)
//!@}

#endif //__BILATERAL5x5_H__