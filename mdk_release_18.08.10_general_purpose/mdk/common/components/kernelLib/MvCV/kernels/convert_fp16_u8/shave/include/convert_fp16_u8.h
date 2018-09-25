#ifndef __CONVERT_FP16_U8_H__
#define __CONVERT_FP16_U8_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Convert a 16-bit floating point line to 8-bit line, with saturation
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

MVCV_FUNC(void, mvcvConvert_fp16_u8, half* in, u8* out, u32 width)
//!@}

#endif //__CONVERT_FP16_U8_H__
