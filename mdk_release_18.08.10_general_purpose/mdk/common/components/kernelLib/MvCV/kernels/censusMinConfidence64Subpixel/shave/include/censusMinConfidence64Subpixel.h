#ifndef __CENSUS_MIN_CONFIDENCE_64_SUBPIXEL__
#define __CENSUS_MIN_CONFIDENCE_64_SUBPIXEL__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMinConfidence64Subpixel   - Compute minimum disparity with subpixel precision considering its left and right neighbour in the cost vector
/// @param[in] in            - input matching cost
/// @param[out] out          - output disparity (minimum position) with subpixel precision
/// @param[out] outm         - output confidence (minimum value)
/// @param[in] width         - line width
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMinConfidence64Subpixel, u8 *in, half *out, u8 *outm, u32 width)
//!@}
    
#endif //__CENSUS_MIN_CONFIDENCE_64_SUBPIXEL__
    
