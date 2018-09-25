#ifndef __ConvertFp16ToQ4_H__
#define __ConvertFp16ToQ4_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// convert fp16 values to Q4 (fixed point with 4 fractional bits)
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

MVCV_FUNC(void, mvcvConvertFp16ToQ4, half* in, u16* out, u32 width)
//!@}

#endif //__ConvertFp16ToQ4_H__
