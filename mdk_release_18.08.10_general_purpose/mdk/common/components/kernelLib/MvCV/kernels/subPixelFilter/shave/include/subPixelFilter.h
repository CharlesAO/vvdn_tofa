#ifndef __SUBPIXEL_FILTER_H__
#define __SUBPIXEL_FILTER_H__

#include <mv_types.h>
#include <mvcv_macro.h>
//!@{
/// mvcvSubPixelFilter              - is a filter used to increment the precision up to a subpixel level of an integer disparity map. 
///                                   This is done by linear/quadratic interpolation of the winning cost with the neighbouring costs.
///                                   The function that performs the interpolation is stored in a compressed format in lutToUse
/// @param[in] inputDisparityMap    - A u8 array of length width containing a row of a integer disparity map
/// @param[in] inputCostVolume      - A u8 array of length width*maxDisp containing the maxDisp costs associated to each element of inputDisparityMap
/// @param[out] outputDisparityMap  - A u16 array of length width containing a row of a disparity map stored in fixed point format (16-numFractionalBit integer part, numFractionalBit decimal part)
/// @param[in] width                - The length of the input and output array.
/// @param[in] maxDisp              - The number of costs associated to each element of inputDisparityMap
/// @param[in] numFractionalBit     - The number of fractional bits desired in the output fixed point notation format
/// @param[in] lutToUse             - An array of usually 4096 elements that stores the fractional offsets to add to the integer disparity map
/// @return    Nothing

MVCV_FUNC(void, mvcvSubPixelFilter, u8* inputDisparityMap, u8* inputCostVolume, u16* outputDisparityMap, u32 width, u8 maxDisp, u8 numFractionalBit, u8 * lutToUse)
//!@}
    
#endif //__SUBPIXEL_FILTER_H__
