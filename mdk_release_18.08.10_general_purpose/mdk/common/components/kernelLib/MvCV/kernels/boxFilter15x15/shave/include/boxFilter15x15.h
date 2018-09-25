#ifndef __BOXFILTER15x15_H__
#define __BOXFILTER15x15_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs average on 15x15 kernel size
/// @param[in] in        - Input lines
/// @param[out] out      - Output lines
/// @param[in] normalize - 1 for normalizing to kernel size, 0 otherwise  
/// @param[in] width     - Width of input line

MVCV_FUNC(void, mvcvBoxfilter15x15, u8** in, u8** out, u32 normalize, u32 width)
//!@}

#endif //__BOXFILTER15x15_H__
