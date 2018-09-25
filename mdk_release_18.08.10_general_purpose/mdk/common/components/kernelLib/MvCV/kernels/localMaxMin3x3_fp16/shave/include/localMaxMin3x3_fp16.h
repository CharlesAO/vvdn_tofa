#ifndef __LOCALMAXMIN3X3_H__
#define __LOCALMAXMIN3X3_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel will find the points which are minimums or maximums in their 3x3 zone.
/// The points from the middle line are compared to their neighbors
/// @param[in]  inBuffer 		- Address of the fp16 image buffer. This buffer should have a size of 3 * width * sizeof(fp16) bytes (3 lines)
/// @param[in]  width 			- Line width in pixels as u32
/// @param[out] minLocationList - u32 buffer, where the function will save the X coordinates of the minimums found
/// @param[out] maxLocationList - u32 buffer, where the function will save the X coordinates of the maximums found
/// @param[out] minCount 		- The function will save the number of minimums found to this address
/// @param[out] maxCount 		- The function will save the number of maximums found to this address
MVCV_FUNC(void, mvcvLocalMaxMin3x3_fp16, half** inBuffer, u32 width,
          u32 minLocationList[], u32 maxLocationList[], u32* minCount,
          u32* maxCount);
//!@}


#endif //__LOCALMAXMIN3X3_H__
