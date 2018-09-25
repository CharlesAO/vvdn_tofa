#ifndef __CENSUS_MATCHING_PYR_H__
#define __CENSUS_MATCHING_PYR_H__


#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMatching        - performs an XOR operation between pixel one pixel in *in1 and 7 pixels from *in2, based on predicted disparities, and counts up how many values of 1 are in the result
/// @param[in] in1            - pointer to input lines of the left image
/// @param[in] in2            - pointer to input lines of the right image
/// @param[in] predicted      - pointer to predicted disparities 
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMatchingPyr, u32 *in1, u32 *in2, u8 *predicted, u8 *out, u32 width)
//!@}
	
#endif //__CENSUS_MATCHING_PYR_H__