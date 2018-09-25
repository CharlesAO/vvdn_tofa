#ifndef __BOXFILTER9x9_H__
#define __BOXFILTER9x9_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs average on 9x9 kernel size
/// @param[in] in        - Input lines
/// @param[out] out      - Output lines
/// @param[in] normalize - 1 for normalizing to kernel size, 0 otherwise 
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvBoxfilter9x9, u8** in, u8** out, u32 normalize, u32 width)
//!@}

#endif //__BOXFILTER9x9_H__