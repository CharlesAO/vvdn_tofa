#ifndef __CONVERT_U8_FP16_H__
#define __CONVERT_U8_FP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Convert a 8-bit line to 16-bit floating point line
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

MVCV_FUNC(void, mvcvConvert_u8_fp16, u8* in, half* out, u32 width)
//!@}

#endif //__CONVERT_U8_FP16_H__
