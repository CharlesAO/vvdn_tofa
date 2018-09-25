///
/// @file      mvispScale2xBilinHV025_U16ToU16.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            U16 in and out
///

#ifndef __mvispScale2xBilinHV025_U16ToU16_H__
#define __mvispScale2xBilinHV025_U16ToU16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvispScale2xBilinHV025_U16ToU16 kernel
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvispScale2xBilinHV025_U16ToU16, u16** in, u16** out, u32 inWidth)

//!@}

#endif //__mvispScale2xBilinHV025_U16ToU16_H__