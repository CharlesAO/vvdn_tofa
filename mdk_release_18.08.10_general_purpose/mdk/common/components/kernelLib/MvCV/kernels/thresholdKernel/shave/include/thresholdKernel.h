#ifndef __THRESHOLDKERNEL_H__
#define __THRESHOLDKERNEL_H__
#include <mv_types.h>
#include <mvcv_macro.h>

enum
{
    Thresh_To_Zero	     = 0,
    Thresh_To_Zero_Inv   = 1,
    Thresh_To_Binary     = 2,
    Thresh_To_Binary_Inv = 3,
    Thresh_Trunc         = 4
    //THRESH_MASK        = CV_THRESH_MASK,
    //THRESH_OTSU        = CV_THRESH_OTSU
};

//!@{
/// This kernel computes the output image based on a threshold value and a threshold type
/// @param[in] in             - Input lines
/// @param[out] out           - Output lines
/// @param[in] width          - Width of the input line
/// @param[in] height         - Height of the input line
/// @param[in] thresh         - Threshold value
/// @param[in] thresh_type    - One of the 5 available thresholding types:
///                           - Thresh_To_Zero: values below threshold are zeroed
///                           - Thresh_To_Zero_Inv: opposite of Thresh_To_Zero
///                           - Thresh_To_Binary: values below threshold are zeroed and all others are saturated to pixel max value
///                           - Thresh_To_Binary_Inv: opposite of Thresh_To_Binary
///                           - Thresh_Trunc: values above threshold are given threshold value
///                           - Default mode: Thresh_Trunc
/// @return    Nothing

MVCV_FUNC(void, mvcvThresholdKernel, u8** in, u8** out, u32 width, u32 height, u8 thresh, u32 thresh_type)

//!@}

#endif //__THRESHOLDKERNEL_H__