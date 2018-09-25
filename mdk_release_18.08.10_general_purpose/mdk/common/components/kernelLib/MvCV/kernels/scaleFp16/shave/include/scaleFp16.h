#ifndef __SCALEFP16_H__
#define __SCALEFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{	
/// This kernel performs the multiplcation of the input values by the scaling factor
/// @param[in] in        - Input array, in fp16 format
/// @param[out] out      - Output array, in fp16 format
/// @param[out] out      - scaling factor, in fp16 format
/// @param[in] width     - Width of input array

MVCV_FUNC(void, mvcvScaleFp16, half** in, half** out, half scale, u32 width)
//!@}

#endif //__SCALEFP16_H__
