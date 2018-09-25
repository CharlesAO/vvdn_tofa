#ifndef __THRESHOLDBINARYRANGE_H__
#define __THRESHOLDBINARYRANGE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel set output to 0xFF if source(x,y) is in specified range, otherwise  output = 0.
/// @param[in] in                - Input lines    
/// @param[out] out              - Output lines  
/// @param[in] lowerValue        - LowerValue   
/// @param[in] upperValue        - UpperValue   
/// @param[in] width             - Width of input line

MVCV_FUNC(void, mvcvThresholdBinaryRange, u8** in, u8** out,  u8 lowerValue, u8 upperValue, u32 width)

#endif //__THRESHOLDBINARYRANGE_H__