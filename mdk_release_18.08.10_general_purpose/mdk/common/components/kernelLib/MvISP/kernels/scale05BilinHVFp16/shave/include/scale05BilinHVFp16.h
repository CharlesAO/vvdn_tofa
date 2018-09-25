///
/// @file  scale05BilinHVFp16.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            Fp16 in and out
///

#ifndef __SCALE05BILINHVFP16_H__
#define __SCALE05BILINHVFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This is a downscale filter with 0.5 factor and 2 input lines
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- array of pointers to output lines
/// @param[in] width		- width of input line
/// @return    Nothing

MVCV_FUNC(void, mvispScale05BilinHVFp16, half **in, half **out, u32 width)

//!@}

#endif //__SCALE05BILINHVFP16_H__