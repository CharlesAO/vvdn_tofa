#ifndef __THRESHOLD_DISPARITY_FP16__
#define __THRESHOLD_DISPARITY_FP16__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvThresholdDisparityFp16   - filters and counts unreliable pixels in the input confidence map
/// @param[in] input             - confidence map
/// @param[in] countThresPixels  - number of pixels with unreliable disparity
/// @param[in] width             - image line width
/// @param[in] threshold         - pointer to a width x 32 array from path2
/// @param[in] setValue          - new value for unreliable pixels
/// @param[out] output           - disparity map in fp16 precision
/// @return    Nothing

MVCV_FUNC(void, mvcvThresholdDisparityFp16, u8* input, half* output, u32* countThresPixels, u32 width, u32 threshold, half setValue)
//!@}
    
#endif //__THRESHOLD_DISPARITY_FP16__
    
