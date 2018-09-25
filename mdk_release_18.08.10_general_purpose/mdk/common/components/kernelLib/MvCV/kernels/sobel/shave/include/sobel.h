#ifndef __SOBEL_H__
#define __SOBEL_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs sobel edge detection operator
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvSobel, u8** in, u8** out, u32 width)
//!@}

#endif //__SOBEL_H__