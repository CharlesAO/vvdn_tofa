#ifndef __THRESHOLD_DISPARITY__
#define __THRESHOLD_DISPARITY__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvThresholdDisparity   - filters and counts unreliable pixels in the input confidence map
/// @param[in] input             - confidence map
/// @param[in] countThresPixels  - number of pixels with unreliable disparity
/// @param[in] width             - image line width
/// @param[in] threshold         - pointer to a width x 32 array from path2
/// @param[in] setValue          - new value for unreliable pixels
/// @param[out] output           - disparity map
/// @return    Nothing

MVCV_FUNC(void, mvcvThresholdDisparity, u8* input, u8* output, u32* countThresPixels, u32 width, u32 threshold, u32 setValue)
//!@}
    
#endif //__THRESHOLD_DISPARITY__
    