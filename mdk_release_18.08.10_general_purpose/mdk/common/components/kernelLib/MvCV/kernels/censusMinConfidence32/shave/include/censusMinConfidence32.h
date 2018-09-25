#ifndef __CENSUSMINCONFIDENCE32_H__
#define __CENSUSMINCONFIDENCE32_H__
#include <mv_types.h>
#include <mvcv_macro.h>
 
  //!@{
/// mvcvCensusMinConfidence32
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of minimum positions
/// @param[out] outm          - array of computation: scaleFactor * minCost / secondMinCost between minimum value and second minimum
/// @param[in] width          - width of the input lines
/// @return    Nothing
 MVCV_FUNC(void, mvcvCensusMinConfidence32, u8 *input, u8 *minimumPosition, u8* minimumValue, u32 width);
//!@}
	
#endif //__MEMCPY_H__