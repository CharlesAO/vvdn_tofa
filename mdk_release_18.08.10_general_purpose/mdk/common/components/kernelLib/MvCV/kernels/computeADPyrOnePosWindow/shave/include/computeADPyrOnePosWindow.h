#ifndef __COMPUTE_AD_PYR_ONEPOS_WINDOW__
#define __COMPUTE_AD_PYR_ONEPOS_WINDOW__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvComputeADPyrOnePosWindow         - compute absolute difference between each pixel (x, y) from left image and 5 neighbour pixels (x-1, y), (x, y), (x+1, y) from right image
/// @param[in] inputLeft        - pointer to left image
/// @param[in] inputRight        - pointer to right image
/// @param[out] output             - pointer to output - absolute difference
/// @param[in] width              - image width
/// @return    Nothing

MVCV_FUNC(void, mvcvComputeADPyrOnePosWindow, u8* inputLeft, u8* inputRight, u8* output, u32 width)
//!@}
    
#endif //__COMPUTE_AD_PYR_ONEPOS_WINDOW__
