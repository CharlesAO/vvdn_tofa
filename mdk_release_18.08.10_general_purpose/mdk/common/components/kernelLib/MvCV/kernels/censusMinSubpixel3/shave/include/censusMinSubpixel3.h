#ifndef __CENSUS_MIN_SUBPIXEL_3__
#define __CENSUS_MIN_SUBPIXEL_3__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMinSubpixel3   - Compute minimum disparity with subpixel precision considering its left and right neighbour in the cost vector
/// @param[in] in            - input matching cost
/// @param[out] out          - output disparity (minimum position) with subpixel precision
/// @param[out] outm         - output confidence (minimum value)
/// @param[in] width         - line width
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMinSubpixel3, u8 *in, half *out, u8 *outm, u32 width)
//!@}
    
#endif //__CENSUS_MIN_SUBPIXEL_3__
    
