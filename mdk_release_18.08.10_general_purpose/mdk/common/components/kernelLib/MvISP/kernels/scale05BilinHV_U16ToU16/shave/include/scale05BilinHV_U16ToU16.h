///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale to half resolution in each direction with bilinear (sw) kernel; 
///            U16 in / U16 out
///

#ifndef __SCALE05BILINHV_U16ToU16_H__
#define __SCALE05BILINHV_U16ToU16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// scale05BilinHV_U16ToU16 kernel
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvispScale05BilinHV_U16ToU16, u16** in, u16* out, u32 inWidth)
//!@}

#endif //__SCALE05BILINHV_U16ToU16_H__