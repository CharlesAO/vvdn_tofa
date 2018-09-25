///
/// @file      mvispScale2xBilinHV025_U16ToU16.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            Fp16 in and out
///

#ifndef __mvispScale2xBilinHV_Fp16ToFp16_even_H__
#define __mvispScale2xBilinHV_Fp16ToFp16_even_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvispScale2xBilinHV_Fp16ToFp16_even kernel -  This filter performs bilinear upscale with 2 factor; Horizontal- applied on even lines
/// @param[in] in         - Input line
/// @param[in] out        - Output line
/// @param[in] inWidth    - Width of output line

MVCV_FUNC(void, mvispScale2xBilinHV_Fp16ToFp16_even, half** in, half** out, u32 outWidth)

//!@}

#endif //__mvispScale2xBilinHV025_U16ToU16_H__