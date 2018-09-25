#ifndef __CENSUS_MIN65_H__
#define __CENSUS_MIN65_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMin - computes minimum of 65 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMin65, u8 *in, u8 *out, u32 width)
//!@}
	
#endif //__CENSUS_MIN_H__