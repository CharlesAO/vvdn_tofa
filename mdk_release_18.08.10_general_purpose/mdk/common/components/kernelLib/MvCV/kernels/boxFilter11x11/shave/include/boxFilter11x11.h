#ifndef __BOXFILTER11x11_H__
#define __BOXFILTER11x11_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs average on 11x11 kernel size
/// @param[in] in        - Input lines
/// @param[out] out      - Output lines
/// @param[in] normalize - 1 for normalizing to kernel size, 0 otherwise 
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvBoxfilter11x11, u8** in, u8** out, u32 normalize, u32 width)
//!@}

#endif //__BOXFILTER11x11_H__
